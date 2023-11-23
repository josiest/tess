#pragma once
#include <cmath>
#include <concepts>
#include <functional>

namespace tess {

/**
 * A represenatation of a cartesian point.
 *
 * points may be printed, added, subtracted and negated:
 *
 * \code{.cpp}
 * point<int> const p1(1, 2);
 * point<int> const p2(3, 4);
 *
 * std::cout << (p1+p2) << '\n';
 * std::cout << (p1-p2) << '\n';
 * std::cout << (-p1) << '\n';
 * \endcode
 *
 * ```
 * > (4, 6)
 * > (-2, -2)
 * > (-1, -2)
 * ```
 *
 * Note that the generic argument must be a numeric type.
 * That is, it must satisfy
 *
 * \code{.cpp}
 * std::is_arithmetic<T>::value
 * \endcode
 */
template<typename Field>
struct point {
public:
    Field x, y;

    /** Fieldhe zero point */
    static point<Field> const zero;

    /** Fieldhe unit point associated with the direction "right" */
    static point<Field> const right;

    /** Fieldhe unit point associated with the direction "left" */
    static point<Field> const left;

    /** Fieldhe unit point associated with the direction "up" */
    static point<Field> const up;

    /** Fieldhe unit point associated with the direction "down" */
    static point<Field> const down;
};

template<typename Field>
point(Field, Field) -> point<Field>;

template<typename Field> constexpr point<Field> const point<Field>::zero{0, 0};
template<typename Field> constexpr point<Field> const point<Field>::right{1, 0};
template<typename Field> constexpr point<Field> const point<Field>::left{-1, 0};
template<typename Field> constexpr point<Field> const point<Field>::up{0, 1};
template<typename Field> constexpr point<Field> const point<Field>::down{0, -1};
    
/**
 * Calculate the 2D euclidean norm of p as a double.
 *
 * This is equivalent to \f$\sqrt{p_x^2 + p_y^2}\f$
 */
template<typename T>
    double norm(const point<T>& p) noexcept
    {
        return std::sqrt(p.x*p.x + p.y*p.y);
    }

/**
 * Calculate the euclidean norm of p as a float.
 *
 * This is equivalent to \f$\sqrt{p_x^2 + p_y^2}\f$
 */
template<typename T>
    double normf(const point<T>& p) noexcept
    {
        return sqrtf(p.x*p.x + p.y*p.y);
    }

/**
 * Calculate the squared norm of p.
 *
 * This is equivalent to \f$p \cdot p\f$ or \f$p_x^2 + p_y^2\f$.
 */
template<typename T>
    T sqnorm(const point<T>& p) noexcept
    {
        return p.x*p.x + p.y*p.y;
    }

template<typename T>
    point<T> operator+(const point<T>& p, const point<T>& q)
    {
        return point{ p.x+q.x, p.y+q.y };
    }

template<typename T>
    point<T> operator-(const point<T>& p)
    {
        return point{ -p.x, -p.y };
    }

template<typename T>
    point<T> operator-(const point<T>& p, const point<T>& q)
    {
        return p + (-q);
    }

template<typename T>
    bool operator==(const point<T>& a, const point<T>& b)
    {
        return a.x == b.x && a.y == b.y;
    }
}

namespace std {

template <typename T>
    struct hash<tess::point<T>> {
        size_t operator()(const tess::point<T>& p) const {
            std::hash<double> dhash;
            std::size_t const px = dhash(static_cast<double>(p.x));
            std::size_t const py = dhash(static_cast<double>(p.y));
            return px ^ (py + 0x9e3779b9 + (px << 6) + (px >> 2));
        }
    };

}
