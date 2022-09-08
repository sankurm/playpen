//$ g++ -std=c++20 -O3 -Werror -Wall -Wpedantic power_of_2.cpp -o power_of_2
#include <bitset>
#include <bit>
#include <concepts>

constexpr bool is_pow2_bit_counting_loop(std::unsigned_integral auto n) {
    int no_bits = 0;
    for (auto i = 0u; i < sizeof(decltype(n)); ++i) {
        if ((n & (1u << i)) == n) ++no_bits;
    }
    return no_bits == 1;
}

constexpr bool is_pow2_match_with_powers(std::unsigned_integral auto n) {
    for (auto i = 0u; i < sizeof(decltype(n)); ++i) {
        if (n == (1u << i)) return true;
    }
    return false;
}

constexpr bool is_pow2_knr(std::unsigned_integral auto n) {
  return (n & (n - 1)) == 0;
}

constexpr bool is_pow2_bitset(std::unsigned_integral auto n) {
    std::bitset<sizeof(decltype(n))> bs(n);
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

static_assert(is_pow2_bit_counting_loop(2u));
static_assert(is_pow2_match_with_powers(4u));
static_assert(is_pow2_knr(8u));
//static_assert(is_pow2_bitset(16u));   //bitset::count() isn't constexpr
static_assert(is_pow2_popcount(32u));
static_assert(is_pow2_has_single_bit(64u));
    

int main(int argc, char** argv) {
    int ret = 0;
    if (is_pow2_bit_counting_loop(2u)) ++ret;
    if (is_pow2_match_with_powers(2u)) ++ret;
    if (is_pow2_knr(2u)) ++ret;
    if (is_pow2_bitset(2u)) ++ret;
    if (is_pow2_popcount(2u)) ++ret;
    if (is_pow2_has_single_bit(2u)) ++ret;
    return ret;
}
