#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <iterator>
#include <random>
#include <chrono>
#include <thread>
#include <functional>

using namespace std::chrono_literals;

//algorithm
template<typename RandomIt, typename UnaryPredicate, typename Compare>
RandomIt partition_sort_2ndpart(RandomIt first, RandomIt last, UnaryPredicate pred, Compare comp) {
	auto partition_point = std::partition(first, last, pred);
	std::sort(partition_point, last, comp);
	return partition_point;
}

//Deliverable
struct packet
{
	uint64_t time_ns;
	uint8_t* data;
};
auto packet_time_less = [](const packet& p1, const packet& p2) {
	return p1.time_ns < p2.time_ns;
};

auto get_time_ns = [](const auto& p) { return p.time_ns; };

template<typename T, typename TimeGetter = decltype(get_time_ns), uint64_t latency_duration = 100>
class Orderer
{
	std::vector<T> cont;
	TimeGetter timeGetter;

	auto gather(uint64_t send_time) {
		auto is_not_ready = [send_time, &tg = timeGetter](const T& t) { 
			return tg(t) > send_time; 
		};
		
		auto part_point = partition_sort_2ndpart(begin(cont), end(cont), 
			is_not_ready, packet_time_less);
		
		return std::make_pair(part_point, end(cont));
	}

	public:
	Orderer(TimeGetter& getter) : timeGetter(getter) {
	}
	void insert(const T& t) {
		cont.push_back(t);
	}

	template<typename Callable>
	void invoke_for_ready(uint64_t now, Callable&& call) {
		uint64_t send_time = now - latency_duration;
		auto [part_begin, part_end] = gather(send_time);

		std::cout << "Invoking for " << std::distance(part_begin, part_end) << 
			" at time " << send_time << '\n';
		std::for_each(part_begin, part_end, [&call](const packet& p) {
			std::invoke(std::forward<Callable>(call), p);
		}
		);
		std::cout << '\n';
		
		cont.erase(part_begin, part_end);
		std::cout << "After send, " << cont;
	}
};

//test
std::random_device rand_dev;
std::mt19937 generator(rand_dev());

std::uniform_int_distribution<std::mt19937::result_type> rand_ns(1, 1000); // distribution in range [1, 1000]
std::uniform_int_distribution<std::mt19937::result_type> rand_count(1, 100); // distribution in range [1, 100]
std::uniform_int_distribution<std::mt19937::result_type> rand_ns_send(1, 1000); // distribution in range [1, 10]

std::ostream& operator<<(std::ostream& out, const std::vector<packet>& vec) {
	std::cout << "Container has " << vec.size() << " packets\n";
	std::transform(begin(vec), end(vec), 
		std::ostream_iterator<uint64_t>(std::cout, ", "), 
		[](const packet& p){ return p.time_ns; }
	);
	std::cout << '\n';
}

packet get_packet(uint64_t now) {
	return packet{now + rand_ns(generator), 0};
}

void send_packet(const packet& p) {
	std::cout << p.time_ns << ", ";
}

auto gather_packets(std::vector<packet>& vec, uint64_t send_time) {
	auto packets_not_ready = [send_time](const packet& p) { return p.time_ns > send_time; };
    
	auto part_point = partition_sort_2ndpart(begin(vec), end(vec), 
		packets_not_ready, packet_time_less);
    
	return part_point;
}

void send(std::vector<packet>& vec, uint64_t send_time) {
	auto part_point = gather_packets(vec, send_time);

	std::cout << "Sending " << std::distance(part_point, end(vec)) << 
		" packets for time " << send_time << '\n';
	std::for_each(part_point, end(vec), send_packet);
	std::cout << '\n';
    
	vec.erase(part_point, end(vec));
	std::cout << "After send, " << vec;
}

template<typename Container>
void receive_n_packets(Container& c, const int n, const uint64_t now) {
	std::cout << "Receiving " << n << " packets with now = " << now << '\n';
	std::generate_n(std::back_inserter(c), n, [now]() { return get_packet(now); });
	std::cout << c;
}

//template<typename Container>
std::vector<packet> generate_n_packets(const uint64_t now, const int n) {
	std::cout << "Receiving " << n << " new packets at now = " << now << '\n';
	std::vector<packet> new_packs;
	std::generate_n(std::back_inserter(new_packs), n, [now]() { return get_packet(now); });
	std::cout << new_packs;
	return new_packs;
}

int main(int argc, char** argv) {
	Orderer<packet> ord(get_time_ns);

	for (uint64_t now = 0; now < 10'000; now += 1000) {
		int n = rand_count(generator);

		auto new_packs = generate_n_packets(now, n);
		for (const auto& p : new_packs) {
			ord.insert(p);
		}

		uint64_t send_time = now + rand_ns_send(generator);
		ord.invoke_for_ready(send_time, send_packet);

		std::this_thread::sleep_for(2s);
		std::cout << '\n';
	}
}
