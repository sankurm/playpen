#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>

using namespace std::placeholders;

using range = std::pair<int, int>;
using packet = std::pair<int, int>;

const auto START = 0;
const auto END = 999;
const auto expected_range = range{START, END};
const auto test_packets = std::vector<packet>{{0, 99}, {100, 199}, {50, 99}, {250, 299}, {220, 230}, {210, 210}, {227, 227}, {500, 699}, {200, 249}, {350, 399}, {225, 375}, {450, 460}, {400, 699}, {700, 799}, {800, 899}, {900, 999}};

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

//For logging
template<typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U> r) {
	out << '{' << r.first << ", " << r.second << '}';
	return out;
}
template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
	out << '{';
	std::for_each(begin(v), end(v), [&out](auto item) { out << item << ", "; });
	out << '}';
	return out;
}

//Test functions
packet mock_receive_packet_sequenced() {
	static auto start = 0, end = 99;
	auto p = packet{start, end};
	start += 100;
	end += 100;
	return p;
}
packet mock_receive_packet_reversed() {
	static auto start = 900, end = 999;
	auto p = packet{start, end};
	start -= 100;
	end -= 100;
	return p;
}
packet mock_receive_packet_out_of_order() {
	static int index = 0;
	return test_packets[index++];
}

packet receive_packet() {
	//return mock_receive_packet_sequenced();
	return mock_receive_packet_reversed();
	//return mock_receive_packet_out_of_order();
}

template<typename reception_function>
void test_reception(reception_function receive, const int __iterations = 0)
{
	auto received = range_receiver{};
	auto is_iterations_done = [__iterations](auto no) { return __iterations != 0 && no == __iterations; };

	for (auto i = 0; !received.is_done(expected_range) && !is_iterations_done(i); i++)
	{
		packet p = receive();
		received << p;
		std::cout << i << ": With received packet: " << p << " -> "<< received.ranges.size() << " -> " << received.ranges << std::endl;
	}
	std::cout << "END OF TEST CASE. Is Done: " << std::boolalpha << received.is_done(expected_range) << "\n\n";
}

int main(int /*argc*/, char** /*argv*/) {
	test_reception(mock_receive_packet_sequenced);
	test_reception(mock_receive_packet_reversed);
	test_reception(mock_receive_packet_out_of_order);
	test_reception(mock_receive_packet_sequenced, 6);
	test_reception(mock_receive_packet_reversed, 4);
	test_reception(mock_receive_packet_out_of_order, 5);

	return 0;
}
