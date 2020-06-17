#ifndef HEXES_HEX_HPP
#define HEXES_HEX_HPP

#include <ostream>
#include <type_traits>  // is_arithmetic
#include <stdexcept>    // invalid_argument
#include <cmath>        // abs, sqrt, sqrtf
#include <algorithm>    // max_element, transform
#include <numeric>      // numeric
#include <valarray>

namespace hax {

/**
 * A representation of a hexagonal coordinate.
 *
 * Hexes may be printed, added, subtracted and negated
 *
 * \code{.cpp}
 * Hex<int> const h1(1, 2);
 * Hex<int> const h2(3, 4);
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
template<typename T>
    class Hex {
        T _q, _r;
    
    public:

        /**
         * Create the hex <q, r, -q-r>.
         * \throws std::invalid_argument if q or r are infinite or NaN
         */
        constexpr Hex(T q, T r)
            :_q(q), _r(r)
        {
            static_assert(std::is_arithmetic<T>::value,
                          "T must be a numeric type");

            if (std::isnan(q) || !std::isfinite(q)) {
                throw std::invalid_argument{"q must be a valid number"};
            }
            if (std::isnan(r) || !std::isfinite(r)) {
                throw std::invalid_argument{"r must be a valid number"};
            }
        }

        /** The q component of this hex. */
        T q() const noexcept { return _q; }

        /** The r component of this hex. */
        T r() const noexcept { return _r; }

        /** The s component of this hex. */
        T s() const noexcept { return -_q-_r; }

        /** The zero hex */
        static Hex<T> constexpr zero{0, 0};

        /** The hex associated with the direction "left and up" */
        static Hex<T> constexpr left_up{0, -1};

        /** The hex associated with the direction "forward and left" */
        static Hex<T> constexpr forward_left{1, -1};

        /** The hex associated with the direction "forward and down" */
        static Hex<T> constexpr forward_down{1, 0};

        /** The hex associated with the direction "right and down" */
        static Hex<T> constexpr right_down{0, 1};

        /** The hex associated with the direction "back and right" */
        static Hex<T> constexpr back_right{-1, 1};

        /** The hex associated with the direction "back and up" */
        static Hex<T> constexpr back_up{-1, 0};
    };

/**
 * Calculate the hex norm of h.
 *
 * This is equivalent to \f$\frac{|h_q| + |h_r| + |h_s|}{2}\f$
 */
template<typename T>
    T hex_norm(const Hex<T>& h) noexcept
    {
        return (std::abs(h.q()) + std::abs(h.r()) + std::abs(h.s()))/2;
    }

/**
 * Calculate the hex with the minimum distance to `h` who's components are
 * integers.
 */
template<typename I, typename R>
    Hex<I> hex_round(const hax::Hex<R>& h)
    {
        // convert hex to valarray for easy operations
        std::valarray<R> v{h.q(), h.r(), h.s()};

        // round each component
        auto rv = v;
        std::transform(std::begin(v), std::end(v), std::begin(rv),
                       [](R& e) { return std::round(e); });

        // take the difference between the original and the rounded
        std::valarray<R> dv{std::abs(rv-v)};

        // find the max difference and correct it
        auto i = std::distance(std::begin(dv),
                               std::max_element(std::begin(dv), std::end(dv)));
        rv[i] -= std::accumulate(std::begin(rv), std::end(rv), 0);

        return Hex<I>(static_cast<I>(rv[0]), static_cast<I>(rv[1]));
    }

template<typename T>
    bool operator==(const Hex<T>& a, const Hex<T>& b)
    {
        return a.q()==b.q() && a.r()==b.r();
    }

template<typename T>
    Hex<T> operator+(const Hex<T>& a, const Hex<T>& b)
    {
        return Hex<T>(a.q()+b.q(), a.r()+b.r());
    }

template<typename T>
    Hex<T> operator-(const Hex<T>& h)
    {
        return Hex<T>(-h.q(), -h.r());
    }

template<typename T>
    Hex<T> operator-(const Hex<T>& a, const Hex<T>& b)
    {
        return a + (-b);
    }

template<typename T>
    std::ostream& operator<<(std::ostream& os, const Hex<T>& h)
    {
        os << "<" << h.q() << ", " << h.r() << ", " << h.s() << ">";
        return os;
    }
}

namespace std {

template <typename T>
    struct hash<hax::Hex<T>> {
        size_t operator()(const hax::Hex<T>& h) const {
            hash<double> dhash;
            size_t hq = dhash((double)h.q());
            size_t hr = dhash((double)h.r());
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };

}

#endif
