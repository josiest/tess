#include "grid.hpp"
#include "math.hpp"     // hex_basis, angle_to_vec
#include <valarray>     // valarray, begin(valarray), end(valarray)
#include <numeric>      // accumulate
#include <algorithm>    // min, max
#include <unordered_set>

using namespace std;

namespace hax {

    Grid::Grid(float x, float y, float size, float th)
        :_origin{x, y}, _unit_size(size), _offset(th),
        basis(hex_basis(size, th)), inv_basis(inv_hex_basis(size, th))
    {
    }

    Grid::Grid(float size, float th)
        :_origin{0.0f, 0.0f}, _unit_size(size), _offset(th),
        basis(hex_basis(size, th)), inv_basis(inv_hex_basis(size, th))
    {
    }

    Grid::Grid(float size, HexType T)
        :_origin{0.0f, 0.0f}, _unit_size(size),
        _offset(T == HexType::Pointed? 0.0f : PI/6.0f),
        basis(hex_basis(size, T == HexType::Pointed? 0.0f : PI/6.0f)),
        inv_basis(inv_hex_basis(size, T == HexType::Pointed? 0.0f : PI/6.0f))
    {
    }

    float Grid::unit_size() const noexcept
    {
        return _unit_size;
    }
    void Grid::unit_size(float size)
    {
        if (size <= 0) {
            throw "Grid unit size must be positive";
        }
        basis *= size/_unit_size;
        inv_basis *= size/_unit_size;
        _unit_size = size;
    }

    float Grid::offset() const noexcept
    {
        return _offset;
    }
    void Grid::offset(float th) noexcept
    {
        _offset = th;
        basis = hex_basis(_unit_size, _offset);
        inv_basis = inv_hex_basis(_unit_size, _offset);
    }

    Point Grid::origin() const noexcept
    {
        return Point(_origin[0], _origin[1]);
    }
    void Grid::origin(float x, float y) noexcept
    {
        _origin[0] = x;
        _origin[1] = y;
    }
    void Grid::origin(const Point& p) noexcept
    {
        _origin[0] = p.x;
        _origin[1] = p.y;
    }

    Point Grid::hex_to_cartesian(const Hex& h) const noexcept
    {
        // multiply basis with hex as a 2d vector v
        valarray<float> p(matvec_mul(basis, valarray<float>{h.q, h.r}));
        p += _origin;
        return Point{p[0], p[1]};
    }

    Hex Grid::cartesian_to_hex(float x, float y) const noexcept
    {
        // multiply inverse basis with point after taking origin into account
        valarray<float> p(matvec_mul(inv_basis, valarray<float>{x, y}-_origin));
        return Hex(p[0], p[1]);
    }

    Hex Grid::cartesian_to_hex(const Point& p) const noexcept
    {
        return cartesian_to_hex(p.x, p.y);
    }

    vector<Point> Grid::vertices(const Hex& h) const noexcept
    {
        auto center = hex_to_cartesian(h);
        vector<Point> verts;
        for (int i = 0; i < 6; ++i) {
            // pi/6 is the first pointed-top coorinate
            // each angle is pi/3 radians apart
            // grid may be offset by an angle
            float th = _offset + (2*i + 1)*PI/6.0f;

            auto v = angle_to_vec(th); // unit vector equivalent of theta
            v *= _unit_size;
            v[0] += center.x;
            v[1] += center.y;

            verts.emplace_back(v[0], v[1]);
        }
        return verts;
    }

    unordered_set<Hex> Grid::hex_map(int radius)
    {
        unordered_set<Hex> map;
        for (int q = -radius; q <= radius; ++q) {

            int start = max(-radius, -q-radius);
            int stop = min(radius, -q+radius);
            for (int r = start; r <= stop; ++r) {
                map.insert(Hex(q, r));
            }
        }
        return map;
    }
}
