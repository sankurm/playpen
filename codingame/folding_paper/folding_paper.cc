#include <iostream>
#include <string>

using namespace std;

struct paper_sides
{
    int l = 1;
    int r = 1;
    int u = 1;
    int d = 1;
};

int get(const paper_sides& sides, char side) {
    switch(side) {
        case 'L': return sides.l;
        case 'R': return sides.r;
        case 'U': return sides.u;
        case 'D': return sides.d;
        default: throw;
    }
}

void fold(int& fold_edge, int& opp_edge, int& side1, int& side2) {
    opp_edge += fold_edge;  //opposide edge additionally sees side folded over
    fold_edge = 1;          //folded edge reduces to 1
    side1 *= 2;             //both sides double themselves up
    side2 *= 2;
}

paper_sides fold(paper_sides sides, const string& instructions) {
    for (auto fold_edge : instructions) {
        auto& [l, r, u, d] = sides;
        switch(fold_edge) {
            case 'L': fold(l, r, u, d); break;
            case 'R': fold(r, l, u, d); break;
            case 'U': fold(u, d, l, r); break;
            case 'D': fold(d, u, l, r); break;
            default: throw;
        }
    }
    return sides;
}

int main()
{
    string order;
    getline(cin, order);
    char side;
    cin >> side;

    auto paper = fold(paper_sides{}, order);
    cout << get(paper, side) << endl;
}
