#include <algorithm>

template<typename RandIt, typename UnaryPredicate, typename Compare>
RandIt partition_sort_2ndpart(RandIt first, RandIt last, UnaryPredicate part_pred, Compare comp) {
	auto partition_point = std::partition(first, last, part_pred);
	std::sort(partition_point, last, comp);
	return partition_point;
}
