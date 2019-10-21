#include <iostream>
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
#include <iterator>
#include <numeric>
#include <functional>
#include <sstream>

using namespace std::string_literals;

//Implementation for greater performance
//infinite loop with break, values calculated
auto get_fields_set(const std::string& line, int no_fields) {
    std::vector<bool> fields_present(no_fields, false);
    size_t pos = 0;
    const int last_index = no_fields - 1;
    for (int index = 0; ; ++index) {
        const auto curr = line.find_first_of(',', pos);
        fields_present[index] = (pos != curr && line[pos] != ',');
        //std::cout << "index: " << index << ": " << fields_present[index] << '\n';
        pos = curr + 1;
        if (curr == std::string::npos || pos == line.length() || index == last_index) {
            break;
        }
    }
    return fields_present;
}

/*template<typename Container>
void operator<<(std::ostream& out, const Container& c) {
    std::ostream_iterator<typename Container::value_type> oit(out, ", ");
    std::copy(begin(c), end(c), oit);
}*/
template<typename Container>
std::string to_string(const Container& c) {
    std::ostringstream oss;
    std::ostream_iterator<typename Container::value_type> oit(oss, ", ");
    std::copy(cbegin(c), cend(c), oit);
    return oss.str();
}

template<typename... Args>
void println(std::ostream& out, Args&&... args) {
    (out << ... << args) << '\n';
}

auto get_field_counts(std::istream& in, int no_fields) {
    std::vector<bool> fields_present(no_fields, false);
    for(int index = 0; !in.eof(); ++index) {
        std::string line;
        std::getline(in, line);
		
        auto curr_present = get_fields_set(line, no_fields);
        std::transform(cbegin(curr_present), cend(curr_present), 
            cbegin(fields_present), begin(fields_present), std::bit_or{}
        );
        
        println(std::cout, "Iteration: ", index, 
            ": curr = ", to_string(curr_present), 
            " overall = ", to_string(fields_present));
    }
    return fields_present;
}

//Need lazy views!
auto get_fields(const std::string& line) {
    std::vector<std::string_view> fields;
    //auto out_it = std::back_inserter(fields);   //Need a back_emplacer! 
    size_t pos = 0;
    for (int index = 0; ; ++index) {
        const auto curr = line.find_first_of(',', pos);
        //*out_it++ = sv;
        fields.emplace_back(line.data() + pos, 
            curr != std::string::npos? curr - pos: line.length() - pos);
        //std::cout << "index: " << index << ": " << fields[index] << '\n';
        pos = curr + 1;
        if (curr == std::string::npos || pos == line.length()) {
            break;
        }
    }
    return fields;
}

int main()
{
    std::string header;
    std::getline(std::cin, header);
    auto fields = get_fields(header);
    println(std::cout, "Headers: ", to_string(fields));
    const int no_fields = fields.size();
    //int no_fields = 1 + std::count(begin(header), end(header), ',');
    std::cout << "no_fields = " << no_fields << '\n';
    
    auto fields_present = get_field_counts(std::cin, no_fields);
    println(std::cout, fields_present.size(), " fields found across all entries: ", to_string(fields_present));
    
    return 0;
}

/*Initial working SIMPLE implementation
auto get_fields_set(const std::string& line, int no_fields) {
    std::vector<int> fields_present(no_fields, false);
    size_t pos = 0;
    for (int index = 0; ; ++index) {
        auto curr = line.find_first_of(',', pos);
        if (curr != std::string::npos) {
            //cout << "index: " << index << " = " << curr - pos << '\n';
            if (pos != curr) {
                fields_present[index] = true;
            }
            pos = curr + 1;
            if (pos == line.length()) { //we are now beyond the end
                break;
            }
        } else {
            //cout << "index: " << index << " = npos\n";
            if (line[pos] != ',') {
                fields_present[index] = true;
            }
            break;
        }
    }
    std::cout << "For line " << line << ", fields_present = ";
    ostream_iterator<bool> oit(cout, ", ");
    copy(begin(fields_present), end(fields_present), oit);
    std::cout << '\n';
    return fields_present;
}*/

/*Implementation with algorithms
auto get_field_counts(std::istream& in, int no_fields) {
    std::vector<bool> fields_present(no_fields, false);
    for(int index = 0; !in.eof(); ++index) {
        string line;
        std::getline(in, line);
		
		std::vector<size_t> positions(no_fields);
		size_t pos = 0;
		std::generate(back_inserter(positions), no_fields - 1, 
			[pos = 0, &line]() mutable { 
				return pos = line.find_first_of(',', pos);
			}
		);
		positions.push_back(line.length());
		cout << "positions = ";
		for_each(cbegin(positions), cend(positions), [](size_t n) { cout << n << ' '; });

		std::vector<int> differences;
		std::adjacent_difference(cbegin(positions), cend(positions), back_inserter(differences));
		for_each(cbegin(differences), cend(differences), [](int n) { cout << n << ' '; });
    }
    return fields_present;
}*/

