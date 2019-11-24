#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

inline int count_inversions(const int* matrix, const int n, const int p, const int q, const int value) {
    int count = 0;
    for (int i = 0; i <= p; ++i) {
        for (int j = 0; j <= q; ++j) {
            //cout << i << ' ' << j << ' ' << matrix[n * i + j] << ' ' << value << '\n';
            if (matrix[n * i + j] > value) {
                ++count;
                //cout << i << ' ' << j << ' ' << matrix[n * i + j] << ' ' << value << 
                    //' ' << count << '\n';
            }
        }
    }
    return count;
}

inline int count_inversions(const int* matrix, const int n) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            count += count_inversions(matrix, n, i, j, matrix[n * i + j]);
        }
    }
    return count;
}

int main() {
	int t;
	cin >> t;
	
	for (int i = 0; i < t; ++i) {
	    int n;
	    cin >> n;
	    int matrix[n][n];
	    istream_iterator<int> in(cin);
	    copy_n(in, n * n, (int*)matrix);
	    cout << count_inversions((int*)matrix, n) << '\n';
	}
}
