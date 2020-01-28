template<typename T, uint64_t latency_duration = 100>
class Orderer
{
	public:
	using GetTime = std::function<uint64_t(const T&)>;

	Orderer() = default;
	Orderer(TimeGetter& getter) : get_time(getter) {
	}
	void insert(const T& t) {
		cont.push_back(t);
	}

	template<typename Callable>
	void invoke_for_ready(uint64_t now, Callable&& call) {
		uint64_t send_time = now - latency_duration;
		auto [part_begin, part_end] = gether_ready(send_time);

		std::cout << "Invoking for " << std::distance(part_begin, part_end) << 
			" at time " << send_time << '\n';
		std::for_each(part_begin, part_end, [&call](const auto& t) {
				std::invoke(std::forward<Callable>(call), t);
			}
		);
		std::cout << '\n';
		
		cont.erase(part_begin, part_end);
		std::cout << "After send, " << cont;
	}

	private:
	std::vector<T> cont;
	GetTime get_time;

	auto gether_ready(const uint64_t send_time) {
		auto is_not_ready = [send_time, &get_time](const T& t) { return get_time(t) > send_time; };
		auto time_less = [&get_time](const T& t1, const T& t2) { return get_time(t1) < get_time(t2); };
		
		auto part_point = partition_sort_2ndpart(begin(cont), end(cont), is_not_ready, time_less);
		
		return std::make_pair(part_point, end(cont));
	}
};
