#pragma once
#include "math.hpp"

#include <concepts>
#include <cmath>
#include <functional>

namespace tess {

/**
 * A represenatation of a cartesian point.
 *
 * Points may be added, subtracted and negated:
 *
 * \code{.cpp}
 * constexpr int_point p{ 1, 2 };
 * constexpr int_point q{ 3, 4 };
 *
 * p + q;
 * p - q;
 * \endcode
 */
template<numeric Field>
struct point {
    /** The zero point */
    static constexpr point<Field> zero{ 0, 0 };

    /** The unit point associated with the direction "right" */
    static constexpr point<Field> right{ 1, 0 };

    /** The unit point associated with the direction "left" */
    static constexpr point<Field> left{ -1, 0 };

    /** The unit point associated with the direction "up" */
    static constexpr point<Field> up{ 0, 1 };

    /** The unit point associated with the direction "down" */
    static constexpr point<Field> down{ 0, -1 };

    constexpr auto begin() { return &x; }
    constexpr auto begin() const { return &x; }

    constexpr auto end() { return (&y) + 1; }
    constexpr auto end() const { return (&y) + 1; }

    Field x, y;
};
using ipoint = point<int>;
using fpoint = point<float>;
using dpoint = point<double>;

/**
 * Calculate the 2D euclidean norm of p as a double.
 *
 * This is equivalent to \f$\sqrt{p_x^2 + p_y^2}\f$
 */
template<numeric Field>
double norm(const point<Field>& p) noexcept
{
    return std::sqrt(p.x*p.x + p.y*p.y);
}

/**
 * Calculate the squared norm of p.
 *
 * This is equivalent to \f$p \cdot p\f$ or \f$p_x^2 + p_y^2\f$.
 */
template<numeric Field>
constexpr Field sqnorm(const point<Field>& p) noexcept
{
    return p.x*p.x + p.y*p.y;
}

template<numeric Field>
constexpr point<Field>
operator+(const point<Field>& p, const point<Field>& q)
{
    return { p.x + q.x, p.y + q.y };
}

template<numeric Field>
constexpr point<Field>
operator-(const point<Field>& p, const point<Field>& q)
{
    return { p.x - q.x, p.y - q.y };
}

template<numeric Field>
constexpr point<Field>
operator-(const point<Field>& p)
{
    return { -p.x, -p.y };
}

template<numeric Field>
constexpr bool
operator==(const point<Field>& p, const point<Field>& q)
{
    return p.x == q.x and p.y == q.y;
}
}

template <tess::numeric Field>
struct std::hash<tess::point<Field>> {
    std::size_t operator()(const tess::point<Field>& p) const {
        constexpr std::hash<Field> hash_field;
        const std::size_t px = hash_field(p.x);
        const std::size_t py = hash_field(p.y);
        return px ^ (py + 0x9e3779b9 + (px << 6) + (px >> 2));
    }
};
