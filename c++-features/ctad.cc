//COMPILATION: g++ -std=c++17 ctad.cc
#include <iostream>
#include <memory>
#include <algorithm>
#include <typeinfo>
#include <vector>
#include <variant>

namespace ankur
{
    template<typename T>
    class vector
    {
    public:
        vector(std::initializer_list<T> vals) : _len(vals.size()), _data(new T[vals.size()])
        { std::copy(std::begin(vals), std::end(vals), _data.get()); }
        auto size() const { return _len; }
        const auto begin() const { return _data.get(); }
        const auto end() const { return _data.get() + _len; }
        
    private:
        std::unique_ptr<T[]> _data;
        size_t _len;
    };

    template<typename... Types>
    vector(Types... values) -> vector<std::variant<Types...>>;

    template<typename T>
    void print(const vector<T>& vec) {
        for (const auto& v : vec) {
            std::visit([](const auto& arg) {
                std::cout << "    " << typeid(arg).name() << ":\t" << arg << "\n";
            }, v);
        }
    }
}

struct S {
    int val = 7;
};
std::ostream& operator<<(std::ostream& out, const S s) { return out << s.val; }

int main() {
    S s1{};
    ankur::vector lvec{1, 2.3, true, 'A', "use deduction guide", s1};
    print(lvec);
    
    return 0;
}
