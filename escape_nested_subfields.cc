#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;

//"top","management","policy acpv1,"DC=xyz.com",subtree,children,read","policy v2,"DC=exfo,dc=com",r,r,100"
auto escape_nested_subfields(const std::string& input) {
	std::string output;
	auto outIt = std::back_inserter(output);
	int nesting_level{0};
	auto next = input.cbegin() + 1;
	//Iterate through input looking at 2 chars at a time
	for (auto curr = input.cbegin(); next != input.cend(); ++curr, ++next) {
		//Logic only needed for quotes
		if (*curr == '\"') {
			if ((nesting_level > 1) || (*next != ',' && nesting_level == 1))
				*outIt++ = '\\';

			if (*next == ',') {
				--nesting_level;
			} else {
				++nesting_level;
			}
		}
		*outIt++ = *curr;
	}
	*outIt++ = input.back();	//loop doesn't write last char
	return output;
}

int main() {
	std::string input;
	std::getline(std::cin, input);
	std::cout << "Input: " << input << std::endl;
	std::cout << "Output: " << escape_nested_subfields(input) << std::endl;
	return 0;
}

