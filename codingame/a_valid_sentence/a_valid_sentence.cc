#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>

using namespace std;

int main()
{
	//Read the words
	vector<string> words;
	copy(istream_iterator<string>(cin), istream_iterator<string>{}, back_inserter(words));
    
	//Get words' lengths
	vector<int> lens;
	transform(begin(words), end(words), back_inserter(lens), 
		[](const auto& s) { return s.length(); });
    
	//Rotate to start the sequence with the smallest word
	auto it = min_element(begin(lens), end(lens));
	auto dist = distance(begin(lens), it);
	rotate(begin(lens), begin(lens) + dist, end(lens));
    
	//If all adjacent differences are 1, it is a VALID SENTENCE!
	vector<int> diffs;
	adjacent_difference(begin(lens), end(lens), back_inserter(diffs));
	if (count(next(begin(diffs)), end(diffs), 1) == diffs.size() - 1) {
		//Writing by collecting in oss only to avoid the space at the end of statement - huh!
		ostringstream oss;
		ostream_iterator<string> out(oss, " ");
		auto first = begin(words) + dist;
		copy(first, end(words), out);
		copy(begin(words), first, out);
		auto answer = oss.str();
		cout << answer.substr(0, answer.length() - 1);
	} else {
		cout << "ERROR";
	}
}
