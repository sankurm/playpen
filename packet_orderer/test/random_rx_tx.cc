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

//Random generators
std::random_device rand_dev;
std::mt19937 generator(rand_dev());
std::uniform_int_distribution<std::mt19937::result_type> rand_ns(1, 1000); // distribution in range [1, 1000]
std::uniform_int_distribution<std::mt19937::result_type> rand_count(1, 100); // distribution in range [1, 100]

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

std::vector<packet> generate_n_packets(const uint64_t now, const int n) {
	std::cout << "Receiving " << n << " new packets at now = " << now << '\n';
	std::vector<packet> new_packs;
	std::generate_n(std::back_inserter(new_packs), n, [now]() { return get_packet(now); });
	std::cout << new_packs;
	return new_packs;
}

int main(int argc, char** argv) {
	Orderer<packet> ord;

	for (uint64_t now = 0; now < 10'000; now += 1000) {
		int n = rand_count(generator);

		auto new_packs = generate_n_packets(now, n);
		for (const auto& p : new_packs) {
			ord.insert(p);
		}

		uint64_t send_time = now + rand_ns(generator);
		ord.invoke_for_ready(send_time, send_packet);

		std::this_thread::sleep_for(2s);
		std::cout << '\n';
	}
}
