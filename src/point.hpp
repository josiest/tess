#pragma once

#include <concepts>
#include <type_traits>
#include <stdexcept>
#include <cmath>

namespace tess {

template<typename Field>
concept numeric_field = requires{
    requires std::floating_point<Field> or std::integral<Field>;
    requires not std::same_as<Field, bool>;
};

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
template<numeric_field Field>
struct basic_point {
    /** The zero point */
    static constexpr basic_point<Field> zero{ 0, 0 };

    /** The unit point associated with the direction "right" */
    static constexpr basic_point<Field> right{ 1, 0 };

    /** The unit point associated with the direction "left" */
    static constexpr basic_point<Field> left{ -1, 0 };

    /** The unit point associated with the direction "up" */
    static constexpr basic_point<Field> up{ 0, 1 };

    /** The unit point associated with the direction "down" */
    static constexpr basic_point<Field> down{ 0, -1 };

    const Field x, y;
};
using point = basic_point<float>;
using pointd = basic_point<double>;
using int_point = basic_point<int>;

/**
 * Calculate the 2D euclidean norm of p as a double.
 *
 * This is equivalent to \f$\sqrt{p_x^2 + p_y^2}\f$
 */
template<numeric_field Field>
double norm(const basic_point<Field>& p) noexcept
{
    return std::sqrt(p.x*p.x + p.y*p.y);
}

/**
 * Calculate the squared norm of p.
 *
 * This is equivalent to \f$p \cdot p\f$ or \f$p_x^2 + p_y^2\f$.
 */
template<numeric_field Field>
constexpr Field sqnorm(const basic_point<Field>& p) noexcept
{
    return p.x*p.x + p.y*p.y;
}

template<numeric_field Field>
constexpr basic_point<Field>
operator+(const basic_point<Field>& p,
          const basic_point<Field>& q)
{
    return { p.x + q.x, p.y + q.y };
}

template<numeric_field Field>
constexpr basic_point<Field>
operator-(const basic_point<Field>& p)
{
    return { -p.x, -p.y };
}

template<numeric_field Field>
constexpr basic_point<Field>
operator-(const basic_point<Field>& p,
          const basic_point<Field>& q)
{
    return p + (-q);
}

template<numeric_field Field>
constexpr bool
operator==(const basic_point<Field>& p,
           const basic_point<Field>& q)
{
    return p.x == q.x and p.y == q.y;
}
}

template <tess::numeric_field Field>
struct std::hash<tess::basic_point<Field>> {
    size_t operator()(const tess::basic_point<Field>& p) const {
        hash<double> hash_double;
        const size_t px = hash_double(static_cast<double>(p.x));
        const size_t py = hash_double(static_cast<double>(p.y));
        return px ^ (py + 0x9e3779b9 + (px << 6) + (px >> 2));
    }
};
