#include "packets_receipt_and_completion.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string_view>

const auto START = 0;
const auto END = 999;
const auto expected_range = range{START, END};
const auto out_of_order_packets = std::vector<packet>{
							{0, 99}, {100, 199}, {50, 99}, {250, 299}, 
							{220, 230}, {210, 210}, {227, 227}, {500, 699}, 
							{200, 249}, {350, 399}, {225, 375}, {450, 460}, 
							{400, 699}, {700, 799}, {800, 899}, {900, 999}};

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
struct mock_receive_packet_sequenced {
	int start = 0, end = 99, increment = 100;
	packet operator()() {
		auto p = packet{start, end};
		start += increment;
		end += increment;
		return p;
	}
};

struct mock_receive_packet_from_container {
	int index = 0;
	const std::vector<packet>& packets;
	packet operator()() {
		return packets[index++];
	}
};

auto mock_packet_sequenced_forward = mock_receive_packet_sequenced{0, 99, 100};
auto mock_packet_sequenced_reversed = mock_receive_packet_sequenced{900, 999, -100};
auto mock_packet_out_of_order = mock_receive_packet_from_container{0, out_of_order_packets};

packet mock_receive_packet_out_of_order() {
	static int index = 0;
	return out_of_order_packets[index++];
}

packet receive_packet() {
	return mock_packet_sequenced_reversed();
	//return mock_receive_packet_sequenced();
	//return mock_receive_packet_reversed();
	//return mock_receive_packet_out_of_order();
}

template<typename reception_function>
void test_reception(std::string_view name, reception_function receive, 
	bool expected_is_done, const int __iterations = 0)
{
	std::cout << "TEST " << name << '\n';
	auto received = range_receiver{};
	auto is_iterations_done = [__iterations](auto no) { return __iterations != 0 && no == __iterations; };

	for (auto i = 0; !received.is_done(expected_range) && !is_iterations_done(i); i++)
	{
		packet p = receive();
		received << p;
		std::cout << i << ": With received packet: " << p << " -> "<< 
			received.ranges.size() << " -> " << received.ranges << '\n';
	}
	auto is_done = received.is_done(expected_range);
	std::cout << "END OF TEST " << name << ". Is Done: " << std::boolalpha 
		<< is_done << ", STATUS: " << (is_done == expected_is_done? "PASS": "FAIL")
		<< "\n\n";
}

#define TEST(rx_fun, expected_is_done) \
	{ \
		auto rx_function = rx_fun; \
		test_reception(#rx_fun, rx_function, expected_is_done); \
	}

#define TEST_NO_PACKETS(rx_fun, expected_is_done, no_packets) \
	{ \
		auto rx_function = rx_fun; \
		test_reception(#rx_fun, rx_function, expected_is_done, no_packets); \
	}

int main(int /*argc*/, char** /*argv*/) {
	TEST(mock_packet_sequenced_forward, true);
	TEST(mock_packet_sequenced_reversed, true);
	TEST(mock_packet_out_of_order, true);
	TEST_NO_PACKETS(mock_packet_sequenced_forward, false, 6);
	TEST_NO_PACKETS(mock_packet_sequenced_reversed, false, 4);
	TEST_NO_PACKETS(mock_packet_out_of_order, false, 5);

	return 0;
}
