#pragma once

#include <type_traits>  // is_arithmetic
#include <cmath>        // abs, sqrt, sqrtf
#include <algorithm>    // max_element
#include <unordered_set>
#include <numeric>
#include <valarray>
#include <vector>

#include "math.hpp"

namespace tess {

/**
 * A representation of a hexagonal coordinate.
 *
 * Hexes may be printed, added, subtracted and negated
 *
 * \code{.cpp}
 * hex<int> const h1(1, 2);
 * hex<int> const h2(3, 4);
 *
 * std::cout << (h1+h2) << '\n';
 * std::cout << (h1-h2) << '\n';
 * std::cout << (-h1) << '\n';
 * \endcode
 *
 * ```
 * > <4, 6, -10>
 * > <-2, -2, 4>
 * > <-1, -2, 3>
 * ```
 *
 * Note that the generic argument must be a numeric type.
 * That is, it must satisfy
 *
 * \code{.cpp}
 * std::is_arithmetic<T>::value
 * \endcode
 */
template<numeric Field>
struct hex {
    /** The s component of this hex. */
    Field s() const noexcept { return -q-r; }
    Field q, r;

    /** The zero hex */
    static hex<Field> const zero;

    /** The hex associated with the direction "left and up" */
    static hex<Field> const left_up;

    /** The hex associated with the direction "forward and left" */
    static hex<Field> const forward_left;

    /** The hex associated with the direction "forward and down" */
    static hex<Field> const forward_down;

    /** The hex associated with the direction "right and down" */
    static hex<Field> const right_down;

    /** The hex associated with the direction "back and right" */
    static hex<Field> const back_right;

    /** The hex associated with the direction "back and up" */
    static hex<Field> const back_up;
};

template<numeric Field>
constexpr hex<Field> const hex<Field>::zero{0, 0};
template<numeric Field>
constexpr hex<Field> const hex<Field>::left_up{0, -1};
template<numeric Field>
constexpr hex<Field> const hex<Field>::forward_left{1, -1};
template<numeric Field>
constexpr hex<Field> const hex<Field>::forward_down{1, 0};
template<numeric Field>
constexpr hex<Field> const hex<Field>::right_down{0, 1};
template<numeric Field>
constexpr hex<Field> const hex<Field>::back_right{-1, 1};
template<numeric Field>
constexpr hex<Field> const hex<Field>::back_up{-1, 0};

template<numeric Field>
hex(Field, Field) -> hex<Field>;

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
template<std::integral Integer, std::floating_point Real>
hex<Integer> hex_round(const hex<Real>& h)
{
    // convert hex to valarray for easy operations
    std::valarray<Real> v{h.q, h.r, h.s()};

    // round each component
    auto rv = v;
    std::transform(std::begin(v), std::end(v), std::begin(rv),
                   [](Real e) { return std::round(e); });

    // take the difference between the original and the rounded
    std::valarray<Real> dv{std::abs(rv-v)};

    // find the max difference and correct it
    auto i = std::distance(std::begin(dv),
                           std::max_element(std::begin(dv), std::end(dv)));
    rv[i] -= std::accumulate(std::begin(rv), std::end(rv), 0);

    return hex<Integer>{static_cast<Integer>(rv[0]),
                        static_cast<Integer>(rv[1])};
}

/**
 * Calculate the hex coordinates in a "straight" line segment from `a` to `b`.
 *
 * The coordinates are calculated by finding the the nearest hex coordinates
 * with integer components to the line segment between a and b.
 */
template<std::integral Integer>
std::vector<hex<Integer>>
line(const hex<Integer>& a, const hex<Integer>& b) noexcept
{
    auto lerp = [](double a, double b, double t) {
        return a + (b - a) * t;
    };
    auto hex_lerp = [&lerp](const hex<Integer>& a,
                            const hex<Integer>& b, double t) {

        return hex{ lerp(a.q, b.q, t),
                    lerp(a.r, b.r, t) };
    };

    std::vector<hex<Integer>> tiles{a};
    Integer const n = hex_norm(a-b);
    for (int i = 1; i <= n; i++) {
        tiles.push_back(hex_round<Integer>(hex_lerp(a, b, i/(double)n)));
    }
    return tiles;
}

/**
 * Calculate the set of hex coordinates within radius `r` of `center`.
 *
 * \note A hex coordinate `a` is within radius `r` of `b` if
 *       `hex_norm(a-b) <= r`.
 *
 * \throws std::invalid_argument if r is negative.
 */
template<std::integral Integer>
std::unordered_set<hex<Integer>>
hex_range(const hex<Integer>& center, Integer r)
{
    std::unordered_set<hex<Integer>> tiles;
    for (Integer i = -r; i <= r; ++i) {
        for (Integer j = std::max(-r, -r-i); j <= std::min(r, r-i); j++) {
            tiles.insert(center + hex<Integer>{i, j});
        }
    }
    return tiles;
}

template<numeric Field>
bool operator==(const hex<Field>& a, const hex<Field>& b)
{
    return a.q == b.q and a.r == b.r;
}

template<numeric Field>
hex<Field> operator+(const hex<Field>& a, const hex<Field>& b)
{
    return hex<Field>{a.q+b.q, a.r+b.r};
}

template<numeric Field>
hex<Field> operator-(const hex<Field>& h)
{
    return hex<Field>{-h.q, -h.r};
}

template<numeric Field>
hex<Field> operator-(const hex<Field>& a, const hex<Field>& b)
{
    return a + (-b);
}
}

namespace std {

template <tess::numeric Field>
    struct hash<tess::hex<Field>> {
        size_t operator()(const tess::hex<Field>& h) const {
            hash<double> dhash;
            size_t hq = dhash(static_cast<double>(h.q));
            size_t hr = dhash(static_cast<double>(h.r));
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };

}
