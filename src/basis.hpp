#pragma once

#include <type_traits>
#include <cmath>
#include <stdexcept>
#include <valarray>
#include <concepts>

#include "hex.hpp"
#include "point.hpp"

namespace tess {

enum class HexTop { Flat, Pointed };

template<typename T>
concept numeric = std::integral<T> or std::floating_point<T>;

template<typename Point>
concept point2d = requires(const Point p) {
    Point{};
    Point{p.x, p.y};
    numeric<std::remove_cvref_t<decltype(p.x)>>;
    std::same_as<std::remove_cvref_t<decltype(p.x)>,
                 std::remove_cvref_t<decltype(p.y)>>;
};

template<point2d Point>
using scalar_field_t =
    std::remove_reference_t<decltype(std::declval<Point>().x)>;


template<std::floating_point R, HexTop TopStyle>
/** An abstract data type for converting to and from screen and hex space. */
class Basis {
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
    Basis(R x, R y, R unit_size)

        : _basis{4}, _inverse{4}, x{x}, y{y},
          _unit_size{unit_size}
    {
        R sqrt3 = std::sqrt(R(3));
        if (TopStyle == HexTop::Pointed) {
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
    template<point2d Point>
    Point origin() const noexcept { return Point{x, y}; }

    /** The unit size of this basis in pixels. */
    R unit_size() const noexcept { return _unit_size; }

    /** Convert `hex` to a point in screen space. */
    template<point2d Point>
    Point pixel(const Hex<scalar_field_t<Point>>& hex) const noexcept
    {
        std::valarray<R> hexv{ static_cast<R>(hex.q()),
                               static_cast<R>(hex.r()) };

        std::valarray<R> const hx = _basis[std::slice(0, 2, 1)] * hexv;
        std::valarray<R> const hy = _basis[std::slice(2, 2, 1)] * hexv;

        using Scalar = scalar_field_t<Point>;
        Point const p{ static_cast<Scalar>(std::round(hx.sum())),
                       static_cast<Scalar>(std::round(hy.sum())) };

        return Point{ p.x+static_cast<Scalar>(x),
                      p.y+static_cast<Scalar>(y) };
    }

    /**
     * Convert `p` to a point in hex space.
     *
     * `p` is meant to be in pixel space, and may to not correspond to an exact
     * hex point. As a result, `hex` will likely return a fractional hex that 
     * hould be rounded to represent a meaningful hex coordinate. See
     * `hex_round` for a function that performs this rounding.
     */
    template<point2d Point>
    Hex<R> hex(const Point& p) const noexcept
    {
        using Scalar = scalar_field_t<Point>;
        Point const p2{ p.x-static_cast<Scalar>(x),
                        p.y-static_cast<Scalar>(y) };

        std::valarray<R> pv{ static_cast<R>(p2.x),
                             static_cast<R>(p2.y) };

        auto q = _inverse[std::slice(0, 2, 1)] * pv;
        auto r = _inverse[std::slice(2, 2, 1)] * pv;

        return Hex<R>(q.sum(), r.sum());
    }

    /** Calculate the vertices of `hex` in screen space. */
    template<point2d Point>
    std::vector<Point>
    vertices(const Hex<scalar_field_t<Point>>& hex) const noexcept
    {
        auto center = pixel<Point>(hex);
        std::vector<Point> verts;

        R const pi = std::acos(-R(1));
        R const offset = TopStyle == HexTop::Pointed? pi/R(6) : 0;

        // add each vertex to the list
        for (int i = 0; i < 6; ++i) {

            // calculate the angle of the vertex
            R theta = offset + i * pi/R(3);

            // convert the angle to unit vector, then scale and offset
            std::valarray<R> v{std::cos(theta), std::sin(theta)};
            v *= _unit_size;
            v += std::valarray<R>{ static_cast<R>(center.x),
                                   static_cast<R>(center.y) };

            using Scalar = scalar_field_t<Point>;
            verts.push_back({ static_cast<Scalar>(std::round(v[0])),
                              static_cast<Scalar>(std::round(v[1])) });
        }
        return verts;
    }

private:
    std::valarray<R> _basis;
    std::valarray<R> _inverse;

    R x; R y;
    R _unit_size;
};

template<HexTop TopStyle>
using fbasis = Basis<float, TopStyle>;

using flat_fbasis = Basis<float, HexTop::Flat>;
using pointed_fbasis = Basis<float, HexTop::Pointed>;
}
