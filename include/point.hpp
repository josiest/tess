#ifndef HEXES_POINT_H
#define HEXES_POINT_H

#include <ostream>
#include <type_traits>
#include <stdexcept>
#include <cmath>

namespace hax {

/**
 * A represenatation of a 2D cartesian point.
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
template<typename T>
    class Point {
        T const _x;
        T const _y;
    
    public:
        /**
         * Create a representation of a cartesian point at (x, y)
         *
         * \param x the x-component of the cartesian point.
         * \param y the y-component of the cartesian point.
         *
         * \throws std::invalid_argument if x or y are NaN
         */
        Point(T x, T y) :_x(x), _y(y)
        {
            static_assert(std::is_arithmetic<T>::value,
                          "T must be a numeric type");

            if (std::isnan(x)) {
                throw std::invalid_argument{"x must be a valid number"};
            }
            if (std::isnan(y)) {
                throw std::invalid_argument{"y must be a valid number"};
            }
        }
    
        /**
         * Get the x-component.
         *
         * \return the x-component of this point.
         */
        inline T x() const noexcept { return _x; }
    
        /**
         * Get the y-component.
         *
         * \return the y-component of this point.
         */
        inline T y() const noexcept { return _y; }
    };
    
/**
 * Calculate the norm of p.
 *
 * \return the 2D euclidean norm of p as a double.
 */
template<typename T>
    double norm(const Point<T>& p) noexcept
    {
        return std::sqrt(p.x()*p.x() + p.y()*p.y());
    }

/**
 * Calculate the norm of p.
 *
 * \return the 2D euclidean norm of p as a float.
 */
template<typename T>
    double normf(const Point<T>& p) noexcept
    {
        return sqrtf(p.x()*p.x() + p.y()*p.y());
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
    std::ostream& operator<<(std::ostream& os, const Point<T>& p)
    {
        os << "(" << p.x() << ", " << p.y() << ")";
        return os;
    }
}

#endif
