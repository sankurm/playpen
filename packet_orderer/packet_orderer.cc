#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

struct packet
{
	uint64_t time_ns;
	uint8_t* data;
};
auto time_less_than = [](const packet& p1, const packet& p2) {
	return p1.time_ns < p2.time_ns;
};

std::ostream& operator<<(std::ostream& out, const std::vector<packet>& vec) {
	std::cout << "Container has " << vec.size() << " packets\n";
	std::transform(begin(vec), end(vec), 
		std::ostream_iterator<uint64_t>(std::cout, ", "), 
		[](const packet& p){ return p.time_ns; }
	);
	std::cout << '\n';
}

std::random_device rand_dev;
std::mt19937 generator(rand_dev());

std::uniform_int_distribution<std::mt19937::result_type> rand_ns(1, 1000); // distribution in range [1, 1000]
std::uniform_int_distribution<std::mt19937::result_type> rand_count(1, 100); // distribution in range [1, 100]
std::uniform_int_distribution<std::mt19937::result_type> rand_ns_send(1, 1000); // distribution in range [1, 10]

packet get_packet(uint64_t now) {
	return packet{now + rand_ns(generator), 0};
}

void send_packet(const packet& p) {
	std::cout << p.time_ns << ", ";
}

auto gather_packets(std::vector<packet>& vec, uint64_t send_time) {
	auto packets_to_send = [send_time](const packet& p) { return p.time_ns < send_time; };
    
	auto partition_point = std::partition(begin(vec), end(vec), packets_to_send);
	std::sort(begin(vec), partition_point, time_less_than);
    
	return partition_point;
}

void send(std::vector<packet>& vec, uint64_t send_time) {
	auto partition_point = gather_packets(vec, send_time);

	std::cout << "Sending " << std::distance(begin(vec), partition_point) << 
		" packets for time " << send_time << '\n';
	std::for_each(begin(vec), partition_point, send_packet);
	std::cout << '\n';
    
	vec.erase(begin(vec), partition_point);
	std::cout << "After send, " << vec;
}

template<typename Container>
void receive_n_packets(Container& c, const int n, const uint64_t now) {
	std::cout << "Receiving " << n << " packets with now = " << now << '\n';
	std::generate_n(std::back_inserter(c), n, [now]() { return get_packet(now); });
	std::cout << c;
}

int main(int argc, char** argv) {
	std::vector<packet> vec;

	for (uint64_t now = 0; now < 10'000; now += 1000) {
		int n = rand_count(generator);
		receive_n_packets(vec, n, now);

		uint64_t send_time = now + rand_ns_send(generator);
		send(vec, send_time);

		std::this_thread::sleep_for(5s);
		std::cout << '\n';
	}
}
