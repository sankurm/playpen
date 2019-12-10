#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

int main() {
	int n;
	std::cin >> n;
	std::istream_iterator<int> in(std::cin);
	std::vector<int> nos(n);
	std::copy_n(in, n, nos.begin());
	
	int x_or = nos.back();
	int all_or = 0;
	
	//xor from rbegin gives xor for each subrange i.e. [i]th xor = xor of all nos[i] to nos[n-1]
	std::for_each(nos.rbegin() + 1, nos.rend(), [&x_or, &all_or](const int n){
	    x_or ^= n;
	    all_or |= x_or;
	});
	
	std::cout << all_or << '\n';
}
