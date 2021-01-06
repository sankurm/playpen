#include <string>
#include <iostream>
#include <variant>
#include <vector>
using namespace std;

template<typename... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

auto check_variant_switch() {
    using my_variant = std::variant<int, double, std::string>;
    /*my_variant var{"alphabet"};
    return std::visit([](const auto& value) {
        std::cout << value << '\n';
        return typeid(value).name();
    }, var);*/
    std::vector<my_variant> v = {22, 3.1415, "mogambo", "one more"};
    double total = 0;
    for (const auto& var : v) {
        std::visit(overloaded {
            [&total](const auto& val){ total += val; },
            [&total](const std::string& val){ total += val.size(); }
        }, var);
    }
    return total;
}

int main(int argc, char** argv) {
    //auto ret = check_variant_switch();
    //std::cout << ret << '\n';
    auto total = check_variant_switch();
    std::cout << total << '\n';
    return 0;
}
