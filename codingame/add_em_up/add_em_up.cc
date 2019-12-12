#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    istream_iterator<long> in(cin);
    const int N = *in++;
    
    vector<long> cards;
    cards.reserve(N);
    copy_n(in, N, back_inserter(cards));
    
    long money_spent = 0;
    for (auto it = begin(cards); ++it != end(cards); ) {
        nth_element(it - 1, it, end(cards));    //sort only top 2 elements
        const auto new_card = *(it - 1) + *it;
        money_spent += new_card;
        *it = new_card;
    }
    cout << money_spent << endl;
}
