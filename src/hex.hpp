#pragma once
#include "fields.hpp"
#include <ranges>
#include <valarray>

#include <cmath>        // abs, sqrt, sqrtf
#include <algorithm>    // max_element
#include <iterator>     // distance
#include <numeric>      // accumulate
#include <functional>   // hash

namespace tess {

/**
 * A representation of a hexagonal coordinate.
 *
 * Hexes may be added, subtracted and negated
 *
 * \code{.cpp}
 * const int_hex a{ 1, 2 };
 * const int_hex b{ 3, 4 };
 *
 * a + b;
 * a - b;
 * -a;
 * \endcode
 */
template<numeric_field Field>
class basic_hex {
public:
    /** The zero hex */
    static constexpr basic_hex<Field> zero{0, 0};

    /** The hex associated with the direction "left and up" */
    static constexpr basic_hex<Field> left_up{0, -1};

    /** The hex associated with the direction "forward and left" */
    static constexpr basic_hex<Field> forward_left{1, -1};

    /** The hex associated with the direction "forward and down" */
    static constexpr basic_hex<Field> forward_down{1, 0};

    /** The hex associated with the direction "right and down" */
    static constexpr basic_hex<Field> right_down{0, 1};

    /** The hex associated with the direction "back and right" */
    static constexpr basic_hex<Field> back_right{-1, 1};

    /** The hex associated with the direction "back and up" */
    static constexpr basic_hex<Field> back_up{-1, 0};

    constexpr Field s() const noexcept { return -q-r; }
    Field q, r;
};
using hex = basic_hex<int>;
using hexf = basic_hex<float>;
using hexd = basic_hex<double>;

template<numeric_field Field>
bool operator==(const basic_hex<Field>& a, const basic_hex<Field>& b)
{
    return a.q == b.q and a.r == b.r;
}

template<numeric_field Field>
basic_hex<Field> operator+(const basic_hex<Field>& a,
                           const basic_hex<Field>& b)
{
    return basic_hex{ a.q + b.q, a.r + b.r };
}

template<numeric_field Field>
basic_hex<Field> operator-(const basic_hex<Field>& h)
{
    return basic_hex{ -h.q, -h.r };
}

template<numeric_field Field>
basic_hex<Field> operator-(const basic_hex<Field>& a,
                           const basic_hex<Field>& b)
{
    return a + (-b);
}

/**
 * Calculate the hex norm of h.
 *
 * This is equivalent to \f$\frac{|h_q| + |h_r| + |h_s|}{2}\f$
 */
template<numeric_field Field>
Field hex_norm(const basic_hex<Field>& h) noexcept
{
    return (std::abs(h.q) + std::abs(h.r) + std::abs(h.s()))/2;
}

/**
 * Calculate the hex with the minimum distance to `h` who's components are
 * integers.
 */
template<std::integral Integer, numeric_field Field>
basic_hex<Integer> hex_round(const basic_hex<Field>& h)
{
    namespace ranges = std::ranges;

    // convert hex to valarray for easy operations
    const std::valarray vals{ h.q, h.r, h.s() };

    // round each component
    auto rounded = vals;
    auto round = [](const Field& e) { return std::round(e); };
    ranges::transform(vals, std::begin(rounded), round);

    // take the difference between the original and the rounded
    std::valarray<Field> diff{ std::abs(rounded-vals) };

    // find the max difference and correct it
    auto i = std::distance(std::begin(diff), ranges::max_element(diff));
    rounded[i] -= std::accumulate(std::begin(rounded), std::end(rounded), 0);
    return basic_hex{ static_cast<Integer>(rounded[0]),
                      static_cast<Integer>(rounded[1]) };
}

/**
 * Calculate the hex coordinates in a "straight" line segment from `a` to `b`.
 *
 * The coordinates are calculated by finding the the nearest hex coordinates
 * with integer components to the line segment between a and b.
 */
template<std::integral Field,
         std::output_iterator<basic_hex<Field>> hex_output>

hex_output line(const basic_hex<Field>& a, const basic_hex<Field>& b,
                hex_output into_hexes) noexcept
{
    namespace views = std::views;
    namespace ranges = std::ranges;

    auto lerp = [](double start, double end, double t) {
        return start + (end - start) * t;
    };
    auto hex_lerp = [&lerp](const auto& a, const auto& b, double t) {
        return basic_hex{ lerp(a.q, b.q, t), lerp(a.r, b.r, t) };
    };
    const Field n = hex_norm(a-b);
    auto lerp_round = [&](int i) {
        return hex_round<Field>(hex_lerp(a, b, i/static_cast<double>(n)));
    };
    into_hexes = ranges::copy(views::single(a), into_hexes).out;
    return ranges::copy(views::iota(1, n+1) | views::transform(lerp_round),
                        into_hexes).out;
}

/**
 * Calculate the set of hex coordinates within radius `r` of `center`.
 *
 * \note A hex coordinate `a` is within radius `r` of `b` if
 *       `hex_norm(a-b) <= r`.
 *
 * \throws std::invalid_argument if r is negative.
 */
template<std::integral Field,
         std::output_iterator<basic_hex<Field>> hex_output>
hex_output hex_range(const basic_hex<Field>& center, Field r,
                     hex_output into_hexes)
{
    namespace ranges = std::ranges;
    namespace views = std::views;

    r = std::abs(r);
    for (const auto i : views::iota(-r, r+1)) {
        auto from_center = [&](const auto j) {
            return center + basic_hex{ i, j };
        };
        const auto lo = std::max(-r, -r-i);
        const auto hi = std::min(r, r-i)+1;
        into_hexes = ranges::transform(views::iota(lo, hi),
                                       into_hexes, from_center).out;
    }
    return into_hexes;
}
}

template <tess::numeric_field Field>
struct std::hash<tess::basic_hex<Field>> {
    size_t operator()(const tess::basic_hex<Field>& h) const {
        hash<Field> hash_field;
        size_t hq = hash_field(h.q);
        size_t hr = hash_field(h.r);
        return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
};
