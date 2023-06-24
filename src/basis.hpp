#pragma once
#include "point.hpp"
#include "hex.hpp"

#include <type_traits>
#include <stdexcept>

#include <cmath>
#include <vector>
#include <valarray>

namespace tess {

enum class HexTop { Flat, Pointed };

template<typename R = double, typename I = int>
/** An abstract data type for converting to and from screen space and hex space. */
class Basis {

    std::valarray<R> _basis;
    std::valarray<R> _inverse;
    const basic_point<I> _origin;
    const I _unit_size;
    const HexTop _top;

public:

    /**
     * Create a basis centered at `origin` in screen space.
     *
     * Basis must have a positive `unit_size` measured in pixels. `top`
     * determines if the top of each hex unit is flat or pointed.
     *
     * \throws std::invalid_argument if R is not a floating point type or if
     *                               unit_size is not positive.
     */
    Basis(basic_point<I> origin, I unit_size, HexTop top = HexTop::Pointed)

        : _basis{4}, _inverse{4}, _origin{origin}, _unit_size{unit_size},
          _top{top}
    {
        if (!std::is_floating_point<R>::value) {
            throw std::invalid_argument{"R must be floating point"};
        }
        if (unit_size <= 0) {
            throw std::invalid_argument{"unit_size must be positive"};
        }

        R sqrt3 = std::sqrt(R(3));
        if (top == HexTop::Pointed) {
            _basis = {sqrt3, sqrt3/2, 0, 3/R(2)};
            _inverse = {sqrt3/3, -1/R(3), 0, 2/R(3)};
        }
        else {
            _basis = {3/R(2), 0, sqrt3/2, sqrt3};
            _inverse = {2/R(3), 0, -1/R(3), sqrt3/3};
        }
        _basis *= _unit_size;
        _inverse /= R(_unit_size);
    }

    /** The origin of this basis in screen space (pixels). */
    basic_point<I> origin() const noexcept { return _origin; }

    /** The unit size of this basis in pixels. */
    I unit_size() const noexcept { return _unit_size; }

    /** Convert `hex` to a point in screen space. */
    basic_point<I> pixel(const basic_hex<I>& hex) const noexcept
    {
        std::valarray<R> hexv{R(hex.q), R(hex.r)};

        auto x = _basis[std::slice(0, 2, 1)] * hexv;
        auto y = _basis[std::slice(2, 2, 1)] * hexv;

        basic_point<I> const p{I(std::round(x.sum())), I(std::round(y.sum()))};

        return p + _origin;
    }

    /**
     * Convert `p` to a point in hex space.
     *
     * `p` is meant to be in pixel space, and may to not correspond to an exact
     * hex point. As a result, `hex` will likely return a fractional hex that 
     * hould be rounded to represent a meaningful hex coordinate. See
     * `hex_round` for a function that performs this rounding.
     */
    basic_hex<R> hex(const basic_point<I>& p) const noexcept
    {
        auto p2 = p - _origin;
        std::valarray<R> pv{R(p2.x), R(p2.y)};

        auto q = _inverse[std::slice(0, 2, 1)] * pv;
        auto r = _inverse[std::slice(2, 2, 1)] * pv;

        return basic_hex{ q.sum(), r.sum() };
    }

    /** Calculate the vertices of `hex` in screen space. */
    std::vector<basic_point<I>> vertices(const basic_hex<I>& hex) const noexcept
    {
        auto center = pixel(hex);
        std::vector<basic_point<I>> verts;

        R const pi = std::acos(-R(1));
        R const offset = _top == HexTop::Pointed? pi/R(6) : 0;

        // add each vertex to the list
        for (int i = 0; i < 6; ++i) {

            // calculate the angle of the vertex
            R theta = offset + i * pi/R(3);

            // convert the angle to unit vector, then scale and offset
            std::valarray<R> v{std::cos(theta), std::sin(theta)};
            v *= _unit_size;
            v += std::valarray<R>{R(center.x), R(center.y)};
            verts.emplace_back(I(std::round(v[0])), I(std::round(v[1])));
        }
        return verts;
    }
};
}
