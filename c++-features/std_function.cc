#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <numeric>

using namespace std::string_literals;

auto doubler = [](int n) -> int { return n + n; };
int quadrupler(int n) { return 4 * n; }

std::map<std::string, std::function<int(int)>> dict = {
    {"Doubler"s, {doubler}},
    {"Tripler"s, {[](int i) { return i + i + i; }}},
    {"Quadrupler"s, {quadrupler}},
    {"5 times"s, {[](int i) { return i * 5; }}},
    {"6 times multiplier"s, {[](int i) { return i * 6; }}}
};

std::string get_options(const std::map<std::string, std::function<int(int)>>& dict) {
    //return std::accumulate(begin(dict), end(dict), ""s, [](const std::string& accumulated, const std::pair<std::string, std::function<int(int)>>& new_option) {
    return std::accumulate(begin(dict), end(dict), ""s, [](const std::string& accumulated, const auto& dict_entry) {
        return accumulated + "/" + dict_entry.first;
    });
}

int main()
{
  const int num = 5;
  std::string key;
  std::cout << "Enter the function [" << get_options(dict) << "]: ";
  getline (std::cin, key);
  std::cout << "Calculated value for num " << num << " with \"" << key << "\" is " << dict[key](num) << "\n";
}
