#include <algorithm>
#include <vector>
#include <variant>

//Capture Service
using Ip = std::variant<ipv4, ipv6>;
using Port = uint16_t;
struct Reception_point
{
	Ip ip;
	Port port;
};

//Filter composed of many clauses
//Clauses could be simple matches like ip.protocol == sctp
//logical ones like AND: ip.protocol == sctp && s1.IMSI == 123456789012345 
template<typename Clause>
struct Filter
{
	Clause clause;
	bool match(reassembled_packet& p) {
		return clause.match(p);
	}
};

class Subscriber
{
	Filter f;
	//std::vector<Filter> filters;	//conjoint i.e. AND of all
	Reception_point remote;

	bool does_match_filter(reassembled_packet& p) {
		return f.match(p);
		//return std::all_of(cbegin(filters), cend(filters), [p](Filter f) { return f.match(p); });
	}
	auto send(msg) {
		return remote.send(msg);
	}
};

auto rx_packet(packet& p) {
	buffer_packet(p);

	reassembled_packet msg = reassemble(p);

	if (!msg.is_complete()) {
		return;
	}

	if (process(msg)) {

	}
}

auto process(reassembled_packet& msg) {
	for (auto& subscriber : subscribers) {
		if (subscriber.does_match_filter(msg) == true) {
			subscriber.send(msg);
		}
	}
}
