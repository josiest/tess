#pragma once

#include <ostream>
#include <type_traits>
#include <stdexcept>
#include <cmath>

namespace tess {

/**
 * A represenatation of a cartesian point.
 *
 * Points may be printed, added, subtracted and negated:
 *
 * \code{.cpp}
 * Point<int> const p1(1, 2);
 * Point<int> const p2(3, 4);
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
template<typename T = int>
    class Point {
        T const _x;
        T const _y;
    
    public:
        /**
         * Create a representation of a cartesian point at (x, y).
         * \throws std::invalid_argument if x or y are NaN
         */
        constexpr Point(T x, T y) :_x(x), _y(y) { }

        /** Create a copy of p. */
        constexpr Point(const Point<T> &p) : _x{p.x()}, _y{p.y()} {}
    
        /** The x component of this point. */
        inline T x() const noexcept { return _x; }
    
        /** The y component of this point. */
        inline T y() const noexcept { return _y; }

        /** The zero point */
        static Point<T> const zero;

        /** The unit point associated with the direction "right" */
        static Point<T> const right;

        /** The unit point associated with the direction "left" */
        static Point<T> const left;

        /** The unit point associated with the direction "up" */
        static Point<T> const up;

        /** The unit point associated with the direction "down" */
        static Point<T> const down;
    };

template<typename T> constexpr Point<T> const Point<T>::zero{0, 0};
template<typename T> constexpr Point<T> const Point<T>::right{1, 0};
template<typename T> constexpr Point<T> const Point<T>::left{-1, 0};
template<typename T> constexpr Point<T> const Point<T>::up{0, 1};
template<typename T> constexpr Point<T> const Point<T>::down{0, -1};
    
/**
 * Calculate the 2D euclidean norm of p as a double.
 *
 * This is equivalent to \f$\sqrt{p_x^2 + p_y^2}\f$
 */
template<typename T>
    double norm(const Point<T>& p) noexcept
    {
        return std::sqrt(p.x()*p.x() + p.y()*p.y());
    }

/**
 * Calculate the euclidean norm of p as a float.
 *
 * This is equivalent to \f$\sqrt{p_x^2 + p_y^2}\f$
 */
template<typename T>
    double normf(const Point<T>& p) noexcept
    {
        return sqrtf(p.x()*p.x() + p.y()*p.y());
    }

/**
 * Calculate the squared norm of p.
 *
 * This is equivalent to \f$p \cdot p\f$ or \f$p_x^2 + p_y^2\f$.
 */
template<typename T>
    T sqnorm(const Point<T>& p) noexcept
    {
        return p.x()*p.x() + p.y()*p.y();
    }

template<typename T>
    Point<T> operator+(const Point<T>& p, const Point<T>& q)
    {
        return Point<T>(p.x()+q.x(), p.y()+q.y());
    }

template<typename T>
    Point<T> operator-(const Point<T>& p)
    {
        return Point<T>(-p.x(), -p.y());
    }

template<typename T>
    Point<T> operator-(const Point<T>& p, const Point<T>& q)
    {
        return p + (-q);
    }

template<typename T>
    bool operator==(const Point<T>& a, const Point<T>& b)
    {
        return a.x() == b.x() && a.y() == b.y();
    }

template<typename T>
    std::ostream& operator<<(std::ostream& os, const Point<T>& p)
    {
        os << "(" << p.x() << ", " << p.y() << ")";
        return os;
    }
}

namespace std {

template <typename T>
    struct hash<tess::Point<T>> {
        size_t operator()(const tess::Point<T>& p) const {
            hash<double> dhash;
            size_t px = dhash((double)p.x());
            size_t py = dhash((double)p.y());
            return px ^ (py + 0x9e3779b9 + (px << 6) + (px >> 2));
        }
    };

}
