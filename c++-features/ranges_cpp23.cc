#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <type_traits>
#include <concepts>
#include <ranges>
#include <functional>
#include <string_view>
#include <variant>
#include <format>
#include <list>
#include <iterator>
#include <tuple>

//Given a sorted vector, find the max gap/diff between two consecutive elements
void max_gap() {
    std::vector v = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int max{};
    //auto pairs = v | std::views::adjacent_transform<2>(
    //    [&max](const auto& p) { max = std::max(max, p.second - p.first); });  //adjacent<2>;
    auto pairs = v | std::ranges::views::adjacent<2>;
    std::ranges::for_each(pairs, [&max](const auto& p) { max = std::max(max, p.second - p.first); });
    std::cout << std::format("Max = {}\n\n", max);
}

//Given a sorted vector, output the max gap/diff between two consecutive elements and the number of times it was seen
void max_gap_freq() {
    std::vector v = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int max{}, freq{};
    auto pairs = v | std::ranges::views::adjacent<2>;
    std::ranges::for_each(pairs, [&max, &freq](const auto& p) {
        auto diff = p.second - p.first;
        if (diff > max) {
            max = diff;
            freq = 1;
        } else if (diff == max) {
            ++freq;
        }
    });
    std::cout << std::format("Max = {} with a frequency of {}\n\n", max, freq);
}

//Count of every word in a string
void word_count() {
    std::string s = "Some long string of words as it's only words";
    std::map<std::string, int> freq;
    std::ranges::for_each(std::ranges::views::split(s, ' '),
        [&freq](const auto w) { std::cout << w[0] << ' '; });
}

//Is a string a palindrome?
void palindrome(std::string_view s) {
    std::ranges::reverse_view rv(s);
    auto zipped = std::ranges::views::zip(s, rv);
    auto ret = std::ranges::find_if(zipped,
        [](const auto& pair){ return get<0>(pair) != get<1>(pair); });
    std::cout << std::format("String {} is {} palindrome\n", s, ret == end(zipped)? "a": "NOT a");
}

//Using ranges: Output chars and the number of times they occur in a list
void count_chars_ranges() {
    std::list<char> chars{'a', 'b', 'd', 'a', 'd', 'z', 'g', 'g', 't', 'z', 't'};
    std::map<char, int> counts;
    std::ranges::for_each(chars, [&counts](char c) { counts[c]++; });
    std::ranges::for_each(counts, [](const auto& pair) { std::cout << std::format("count['{}'] = {}\n", pair.first, pair.second); });
    #ifdef __cpp_lib_print
    std::ranges::for_each(counts, [](const auto& pair) { std::print("count['{}'] = {}\n\n", pair.first, pair.second); });
    #endif
    //std::cout << std::format("{}\n", chars);
}

//Using map: Output chars and the number of times they occur in a list
void count_chars_map() {
    std::list<char> chars{'a', 'c', 'd', 'a', 'b'};
    std::map<char, int> counts;
    std::ranges::for_each(chars, [&counts](char c) { counts[c]++; });
    std::ranges::for_each(counts, [](const auto& p){
        std::cout << std::format("{} -> {}\n", p.first, p.second); });
}

//Not using a map: Output chars and the number of times they occur in a list
void count_chars_arr() {
    std::list<char> chars{'a', 'c', 'd', 'a', 'b'};
    std::array<int, 255> carr{};
    for (auto c: chars) {
        carr[c]++;
    }
    for (char c = 0; c < carr.size(); ++c) {
        if (carr[c]) {
            std::cout << char(c) << " is present " << carr[c] << " times\n";
        }
    }
}

int main() {
    max_gap();
    max_gap_freq();

    word_count();
    std::cout << '\n';

    palindrome("Maharashtra");
    palindrome("AMSXSMA");
    std::cout << '\n';

    count_chars_ranges();

    count_chars_map();

    count_chars_arr();
    return 0;
}

