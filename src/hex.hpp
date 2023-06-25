#pragma once
#include "math.hpp"
#include <ranges>

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
template<numeric Field>
class hex {
public:
    /** The zero hex */
    static constexpr hex<Field> zero{0, 0};

    /** The hex associated with the direction "left and up" */
    static constexpr hex<Field> left_up{0, -1};

    /** The hex associated with the direction "forward and left" */
    static constexpr hex<Field> forward_left{1, -1};

    /** The hex associated with the direction "forward and down" */
    static constexpr hex<Field> forward_down{1, 0};

    /** The hex associated with the direction "right and down" */
    static constexpr hex<Field> right_down{0, 1};

    /** The hex associated with the direction "back and right" */
    static constexpr hex<Field> back_right{-1, 1};

    /** The hex associated with the direction "back and up" */
    static constexpr hex<Field> back_up{-1, 0};

    constexpr auto begin() { return &q; }
    constexpr auto begin() const { return &q; }
    constexpr auto end() { return (&r) + 1; }
    constexpr auto end() const { return (&r) + 1; }

    template<std::output_iterator<Field> FieldOutput>
    constexpr FieldOutput
    cube_coordinates(FieldOutput into_coords) const noexcept
    {
        namespace ranges = std::ranges;
        namespace views = std::views;

        into_coords = ranges::copy(begin(), end(), into_coords).out;
        into_coords = ranges::copy(views::single(s()), into_coords).out;
        return into_coords;
    }

    constexpr Field s() const noexcept { return -q-r; }
    Field q, r;
};

using ihex = hex<int>;
using fhex = hex<float>;
using dhex = hex<double>;

template<numeric Field>
bool operator==(const hex<Field>& a, const hex<Field>& b)
{
    return a.q == b.q and a.r == b.r;
}

template<numeric Field>
hex<Field> operator+(const hex<Field>& a, const hex<Field>& b)
{
    return hex{ a.q + b.q, a.r + b.r };
}

template<numeric Field>
hex<Field> operator-(const hex<Field>& a, const hex<Field>& b)
{
    return hex{ a.q - b.q, a.r - b.r };
}

template<numeric Field>
hex<Field> operator-(const hex<Field>& h)
{
    return hex{ -h.q, -h.r };
}

/**
 * Calculate the hex norm of h.
 *
 * This is equivalent to \f$\frac{|h_q| + |h_r| + |h_s|}{2}\f$
 */
template<numeric Field>
Field hex_norm(const hex<Field>& h) noexcept
{
    return (std::abs(h.q) + std::abs(h.r) + std::abs(h.s()))/2;
}

/**
 * Calculate the hex with the minimum distance to `h` who's components are
 * integers.
 */
template<std::integral Integer, numeric Field>
requires std::convertible_to<Field, Integer>
hex<Integer> hex_round(const hex<Field>& h)
{
    namespace views = std::views;
    namespace ranges = std::ranges;

    // convert to cube coordinates so math is easier
    std::array<Field, 3> cube;
    h.cube_coordinates(cube.begin());

    // round each component
    std::array<Field, 3> rounded;
    round(cube, rounded.begin());

    // take the difference between the original and the rounded
    std::array<Field, 3> diff;
    abs(subtract(rounded, cube), diff.begin());

    // find the max difference and correct it
    auto i = std::distance(diff.begin(), ranges::max_element(diff));
    rounded[i] -= std::accumulate(rounded.begin(), rounded.end(), 0);

    hex<Integer> int_hex;
    ranges::copy(views::take(rounded, 2), int_hex.begin());
    return int_hex;
}

/**
 * Calculate the hex coordinates in a "straight" line segment from `a` to `b`.
 *
 * The coordinates are calculated by finding the the nearest hex coordinates
 * with integer components to the line segment between a and b.
 */
template<std::integral Field,
         std::output_iterator<hex<Field>> HexOutput>

HexOutput line(const hex<Field>& a, const hex<Field>& b,
                HexOutput into_hexes) noexcept
{
    namespace views = std::views;
    namespace ranges = std::ranges;

    auto lerp = [](double start, double end, double t) {
        return start + (end - start) * t;
    };
    auto hex_lerp = [&lerp](const auto& a, const auto& b, double t) {
        return hex{ lerp(a.q, b.q, t), lerp(a.r, b.r, t) };
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
         std::output_iterator<hex<Field>> HexOutput>
HexOutput hex_range(const hex<Field>& center, Field r,
                    HexOutput into_hexes)
{
    namespace ranges = std::ranges;
    namespace views = std::views;

    r = std::abs(r);
    for (const auto i : views::iota(-r, r+1)) {
        auto from_center = [&](const auto j) {
            return center + hex{ i, j };
        };
        const auto lo = std::max(-r, -r-i);
        const auto hi = std::min(r, r-i)+1;
        into_hexes = ranges::transform(views::iota(lo, hi),
                                       into_hexes, from_center).out;
    }
    return into_hexes;
}
}

template <tess::numeric Field>
struct std::hash<tess::hex<Field>> {
    size_t operator()(const tess::hex<Field>& h) const {
        hash<Field> hash_field;
        size_t hq = hash_field(h.q);
        size_t hr = hash_field(h.r);
        return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
};
