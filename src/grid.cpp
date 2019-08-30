#include "grid.hpp"
#include "math.hpp" // hex_basis, angle_to_vec
#include <valarray> // valarray, begin(valarray), end(valarray)
#include <numeric>  // accumulate

using namespace std;

namespace hexes {

    Grid::Grid(float x, float y, float size, float th)
        :origin{x, y}, unit_size(size), offset(th),
        basis(hex_basis(size, th)) {}

    Grid::Grid(float size, float th)
        :origin{0.0f, 0.0f}, unit_size(size), offset(th),
        basis(hex_basis(size, th)) {}

    Grid::Grid(float size, HexType T)
        :origin{0.0f, 0.0f}, unit_size(size),
        offset(T == HexType::Pointed? 0.0f : PI/6.0f),
        basis(hex_basis(size, T == HexType::Pointed? 0.0f : PI/6.0f)) {}

    Point Grid::hex_to_cartesian(const Hex& h) const noexcept
    {
        // hex as a 2d vector v
        valarray<float> v{h.q, h.r};

        // dot the first row with v to get x
        auto s = valarray<float>(basis[row(0)]*v);
        float x = accumulate(begin(s), end(s), 0.0f);

        // dot the second row with v to get y
        s = valarray<float>(basis[row(1)]*v);
        float y = accumulate(begin(s), end(s), 0.0f);

        // add the origin
        return Point{x+origin[0], y+origin[1]};
    }

    vector<Point> Grid::vertices(const Hex& h) const noexcept
    {
        auto center = hex_to_cartesian(h);
        vector<Point> verts;
        for (int i = 0; i < 6; ++i) {
            // pi/6 is the first pointed-top coorinate
            // each angle is pi/3 radians apart
            // grid may be offset by an angle
            float th = offset + (2*i + 1)*PI/6.0f;

            auto v = angle_to_vec(th); // unit vector equivalent of theta
            v *= unit_size;
            v[0] += center.x;
            v[1] += center.y;

            verts.emplace_back(v[0], v[1]);
        }
        return verts;
    }
}
