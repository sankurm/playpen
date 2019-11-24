#include <iostream>
#include <algorithm>
#include <cstdint>
#include <iterator>
 
using namespace std;
using i32_it = istream_iterator<int_fast32_t>;
 
inline int count_divisible_by(int_fast32_t no, int_fast32_t divisor) {
	int count = 0;
	for (; no % divisor == 0; no /= divisor) {
		++count;
	}
	return count;
}
 
int main() {
	int no_entries;
	cin >> no_entries;
	int div_by_2 = 0, div_by_5 = 0;

	for_each_n(i32_it(cin), no_entries, [&](int_fast32_t no) {
		div_by_2 += count_divisible_by(no, 2);
		div_by_5 += count_divisible_by(no, 5);
	});
	
	cout << min(div_by_2, div_by_5);
}
