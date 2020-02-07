#include <partition_sort_2ndpart.h>
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>
#include <cstdint>

template<typename T, typename Criterion>
class Orderer
{
	public:
	using Get_criterion = std::function<Criterion(const T&)>;

	//Orderer() = default;
	template<typename Get_criterion_F>
	//std::enable_if_t<std::is_same_v<std::invoke_result<Get_Criterion_F>::type, Criterion>, int> = 0
	Orderer(const Get_criterion_F& get_crit, Criterion crit_lag) : get_criterion(get_crit), lag(crit_lag) {
	}
	void insert(const T& t) {
		cont.push_back(t);
	}

	template<typename Callable>
	int invoke_for_ready(Criterion crit, Callable&& call);

	const auto& get_cont() const {
		return cont;
	}
	
	private:
	std::vector<T> cont;
	Get_criterion get_criterion;// = [](const T& t) { return t.time_ns; };
	Criterion lag;

	auto gather_qualifying(const Criterion invoke_crit);
};

template<typename T, typename Criterion>
template<typename Callable>
int Orderer<T, Criterion>::invoke_for_ready(Criterion crit, Callable&& call) {
	Criterion invoke_crit = crit - lag;
	auto [part_begin, part_end] = gather_qualifying(invoke_crit);

	const int no_entries = std::distance(part_begin, part_end);
	
	std::for_each(part_begin, part_end, [&call](const auto& t) {
			std::invoke(std::forward<Callable>(call), t);
		}
	);
	
	cont.erase(part_begin, part_end);
	return no_entries;
}

template<typename T, typename Criterion>
auto Orderer<T, Criterion>::gather_qualifying(const Criterion invoke_crit) {
	auto does_not_qualify = [invoke_crit, &gc = get_criterion](const T& t) { return gc(t) > invoke_crit; };
	auto crit_less = [&gc = get_criterion](const T& t1, const T& t2) { return gc(t1) < gc(t2); };
	
	auto part_point = partition_sort_2ndpart(begin(cont), end(cont), does_not_qualify, crit_less);
	
	return std::make_pair(part_point, end(cont));
}
