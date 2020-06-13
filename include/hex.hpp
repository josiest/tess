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

template<typename T>
    class Hex {
        T _q, _r;
    
    public:

        /**
         * Create a Hex from axial coordinates.
         *
         * \param q the first component of the axial coordinate.
         * \param r the second component of the axial coordinate.
         *
         * \throws std::invalid_argument if q or r are NaN
         */
        Hex(T q, T r)
            :_q(q), _r(r)
        {
            static_assert(std::is_arithmetic<T>::value,
                          "T must be a numeric type");

            if (std::isnan(q)) {
                throw std::invalid_argument{"q must be a valid number"};
            }
            if (std::isnan(r)) {
                throw std::invalid_argument{"r must be a valid number"};
            }
        }

        /**
         * The q component of this hex.
         *
         * \return this hex's q component.
         */
        T q() const noexcept { return _q; }

        /**
         * The r component of this hex.
         * 
         * \return this hex's r component.
         */
        T r() const noexcept { return _r; }

        /**
         * The s component of this hex.
         *
         * \return this hex's s component.
         */
        T s() const noexcept { return -_q-_r; }

        /** The zero hex */
        static constexpr Hex zero{0, 0};

        /** The hex <1, -1, 0> */
        static constexpr Hex right_back{1, -1};

        /** The hex <0, -1, 1> */
        static constexpr Hex up_back{0, -1};

        /** The hex <-1, 0, 1> */
        static constexpr Hex up_left{-1, 0};

        /** The hex <-1, 1, 0> */
        static constexpr Hex forward_left{-1, 1};

        /** The hex <0, 1, -1> */
        static constexpr Hex forward_down{0, 1};

        /** The hex <1, 0, -1> */
        static constexpr Hex right_down{1, 0};
    };

/**
 * Calculate the norm of h.
 *
 * \return the euclidean norm of h as a double.
 */
template<typename T>
    double norm(const Hex<T>& h) noexcept
    {
        return std::sqrt(h.q()*h.q() + h.r()*h.r() + h.s()*h.s());
    }

/**
 * Calculate the norm of h.
 *
 * \return the euclidean norm of h as a float.
 */
template<typename T>
    float normf(const Hex<T>& h) noexcept
    {
        return sqrtf(h.q()*h.q() + h.r()*h.r() + h.s()*h.s());
    }

/**
 * Calculate the squared norm of h.
 *
 * \return the dot product h.h
 */
template<typename T>
    T sqnorm(const Hex<T>& h) noexcept
    {
        return h.q()*h.q() + h.r()*h.r() + h.s()*h.s();
    }

/**
 * Calculate the hex norm (similar to manhattan distance, but for hexes).
 * This is equivalent to (|h.q| + |h.r| + |h.s|)/2
 *
 * \return the hex norm of h.
 * \post If T is an integral type, cnorm will perform integer division which may
 *       result in rounding.
 */
template<typename T>
    T hex_norm(const Hex<T>& h) noexcept
    {
        return (std::abs(h.q()) + std::abs(h.r()) + std::abs(h.s()))/2;
    }

/**
 * Round this hex's components to the nearest valid integral hex.
 *
 * \param h the hex to round
 * \return the hex with the minimum distance to h whos components are integers.
 */
template<typename R, typename I>
    Hex<I> hex_round(const hax::Hex<R>& h)
    {
        // convert hex to valarray for easy operations
        std::valarray<R> v{h.q(), h.r(), h.s()};

        // round each component
        auto rv = v;
        std::transform(std::begin(v), std::end(v), std::begin(rv),
                       [](R& e) { return std::round(e); });

        // take the difference between the original and the rounded
        std::valarray<float> dv(std::abs(rv-v));

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
    struct hash<hax::Hex> {
        size_t operator()(const hax::Hex<T>& h) const {
            hash<double> dhash;
            size_t hq = dhash((double)h.q());
            size_t hr = dhash((double)h.r());
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };

}

#endif
