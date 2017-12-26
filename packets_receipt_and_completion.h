//#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std::placeholders;

using range = std::pair<int, int>;
using packet = std::pair<int, int>;

//Utility lambdas
auto is_contegious_or_overlapping = [](auto inserted, auto next) {
	return inserted.second + 1 >= next.first;
};
auto is_start_in_range = [](auto the_range, auto range_to_check_in) {
	return the_range.first <= range_to_check_in.second + 1;
};

class range_receiver {
  public:
		std::vector<range> ranges = {};

		range_receiver& operator<<(const packet& p) {
			auto is_packet_start_in_range = std::bind(is_start_in_range, p, _1);
			auto it = std::find_if(begin(ranges), end(ranges), is_packet_start_in_range);

			//Found
			if (it != end(ranges)) {
				auto& curr = *it;
				if (is_contegious_or_overlapping(p, curr)) {
					curr.second = std::max(curr.second, p.second);
					curr.first = std::min(curr.first, p.first);
					consolidate_after_iterator(it);
					return *this;
				}
			}

			auto inserted_it = ranges.insert(it, range{p});
			consolidate_after_iterator(inserted_it);
			return *this;
		}

		template<typename It>
		void consolidate_after_iterator(It inserted_it) {
			//FIND NEXT ONES THAT NEED TO BE GOBBLED UP
			auto &inserted = *inserted_it;

			auto removed_start = std::remove_if(inserted_it + 1, end(ranges),
				[&inserted](auto next) {
					//std::cout << "Current & Consolidating: " << std::make_pair(inserted, next);
					if (is_contegious_or_overlapping(inserted, next)) {
						inserted.second = std::max(inserted.second, next.second);
						return true;
					}
					return false;
				});
			ranges.erase(removed_start, end(ranges));
		}

		bool is_done(const range& expected_range) const {
			return ranges.size() == 1 && ranges.front() == expected_range;
		}
};
