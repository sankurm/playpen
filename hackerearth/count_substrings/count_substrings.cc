#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
	int n;	//no of chars in input string
	cin >> n;
	string s;
	s.reserve(n);
	cin >> s;
	
	int q;	//no of queries
	cin >> q;
	for (int i = 0; i < q; ++i) {
		char x, y;
		cin >> x >> y;

		int sub_starting_x = 0;
		int sub_starting_y = 0;
		int sub_x_to_y = 0, sub_y_to_x = 0;
		for_each(begin(s), end(s), 
			[x, y, &sub_starting_x, &sub_starting_y, &sub_x_to_y, &sub_y_to_x](char c) {
				if (c == x) {
					++sub_starting_x;
					sub_y_to_x += sub_starting_y;
				} else if (c == y) {
					++sub_starting_y;
					sub_x_to_y += sub_starting_x;
				}
			}
		);

		cout << sub_x_to_y + sub_y_to_x << '\n';
	}
}
