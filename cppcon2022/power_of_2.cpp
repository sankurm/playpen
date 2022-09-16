//$ g++ -std=c++20 -O3 -Werror -Wall -Wpedantic power_of_2.cpp -o power_of_2
#include <bitset>
#include <bit>
#include <concepts>

constexpr bool is_pow2_bit_counting_loop(std::unsigned_integral auto n) {
    using uint = decltype(n);
    int no_bits = 0;
    for (auto i = 0u; i < sizeof(uint) * 8; ++i) {
        if (n & (decltype(n)(1) << i)) ++no_bits;
    }
    return no_bits == 1;
}

constexpr bool is_pow2_match_with_powers(std::unsigned_integral auto n) {
    using uint = decltype(n);
    for (auto i = 0u; i < sizeof(uint) * 8; ++i) {
        if (n == (decltype(n)(1) << i)) return true;
    }
    return false;
}

constexpr bool is_pow2_knr(std::unsigned_integral auto n) {
  return (n & (n - decltype(n)(1))) == 0;
}

constexpr bool is_pow2_bitset(std::unsigned_integral auto n) {
    std::bitset<sizeof(decltype(n)) * 8> bs(n);
    return bs.count() == 1;   //NOT constexpr! :(
}

constexpr bool is_pow2_popcount(std::unsigned_integral auto n) {
    return std::popcount(n) == 1;
}

constexpr bool is_pow2_has_single_bit(std::unsigned_integral auto n) {
    return std::has_single_bit(n);
}

//Template Meta-Programming? Use constexpr instead
template<std::unsigned_integral uint, uint N>
struct pow2
{ uint value = (N == 0? uint(1): pow2<uint, N - 1>::value * uint(2)); };
template<std::unsigned_integral uint>
struct is_pow2
{ bool value = false; };

static_assert(is_pow2_bit_counting_loop(32u));
static_assert(is_pow2_match_with_powers(4u));
static_assert(is_pow2_knr(8u));
//static_assert(is_pow2_bitset(16u));   //bitset::count() isn't constexpr
static_assert(is_pow2_popcount(32u));
static_assert(is_pow2_has_single_bit(64u));
    
static_assert(!is_pow2_bit_counting_loop(100u));
static_assert(!is_pow2_match_with_powers(204u));
static_assert(!is_pow2_knr(298u));
//static_assert(!is_pow2_bitset(12621u));   //bitset::count() isn't constexpr
static_assert(!is_pow2_popcount(77u));
static_assert(!is_pow2_has_single_bit(1836474u));

int main(int argc, char** argv) {
    int ret = 0;

    if (is_pow2_bit_counting_loop(2u)) ++ret;
    if (is_pow2_match_with_powers(2u)) ++ret;
    if (is_pow2_knr(2u)) ++ret;
    if (is_pow2_bitset(2u)) ++ret;
    if (is_pow2_popcount(2u)) ++ret;
    if (is_pow2_has_single_bit(2u)) ++ret;

    if (!is_pow2_bit_counting_loop(232u)) ++ret;
    if (!is_pow2_match_with_powers(223u)) ++ret;
    if (!is_pow2_knr(266u)) ++ret;
    if (!is_pow2_bitset(289u)) ++ret;
    if (!is_pow2_popcount(235u)) ++ret;
    if (!is_pow2_has_single_bit(12u)) ++ret;

    return ret;
}
