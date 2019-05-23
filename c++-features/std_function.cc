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
    {"6 times multiplier"s, {[](int i) { return i * 6; }}},
    {"7 fold"s, {[](int i) { return i * 7; }}},
    {"multiply by 8"s, {[](int i) { return i * 8; }}},
    {"x 9"s, {[](int i) { return i * 9; }}},
    {"ten times"s, {[](int i) { return i * 10; }}}
};

//No std::to_string that takes in a std::string and gives back a string! huh! 
//to_string(const std::string&) is an identity function
const std::string& to_string(const std::string& s) { return s; }

//Generic function to get options from to_string(first) of the Map
template<typename Map>
std::string get_options(const Map& dict) {
    return std::accumulate(begin(dict), end(dict), ""s, [](const std::string& accumulated, const auto& dict_entry) {
        return accumulated.length()? accumulated + "/" + to_string(dict_entry.first): to_string(dict_entry.first);
    });
}

int main()
{
  std::cout << sizeof(std::function<const std::string(int*)>) << '\n';  //32 on cpp.sh
  std::cout << sizeof(std::function<int&(float)>) << '\n';  //32 on cpp.sh
  
  const int num = 5;
  std::string key;
  std::cout << "Enter the function [" << get_options(dict) << "]: ";
  getline (std::cin, key);
  std::cout << "Calculated value for num " << num << " with \"" << key << "\" is " << dict[key](num) << "\n";
}
