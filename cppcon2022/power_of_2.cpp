#include <bitset>
#include <bit>

bool is_pow2_bit_counting_loop(unsigned n) {
    int no_bits = 0;
    for (auto i = 0u; i < sizeof(unsigned); ++i) {
        if ((n & (1u << i)) == n) ++no_bits;
    }
    return no_bits == 1;
}

bool is_pow2_match_with_powers(unsigned n) {
    for (auto i = 0u; i < sizeof(unsigned); ++i) {
        if (n == (1u << i)) return true;
    }
    return false;
}

bool is_pow2_knr(unsigned n) {
  return (n & (n - 1)) == 0;
}

bool is_pow2_bitset(unsigned n) {
    std::bitset<sizeof(unsigned)> bs(n);
    return bs.count() == 1;
}

bool is_pow2_popcount(unsigned n) {
    return std::popcount(n) == 1;
}

bool is_pow2_has_single_bit(unsigned n) {
    return std::has_single_bit(n);
}
