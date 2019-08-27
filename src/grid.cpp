#include "grid.hpp"
#include "math.hpp" // hex_basis
#include <valarray> // valarray, begin(valarray), end(valarray)
#include <numeric>  // accumulate

using namespace std;

namespace hexes {

    Grid::Grid(float x, float y, float size, float th)
        :origin{x, y}, basis(hex_basis(size, th)) {}

    Grid::Grid(float size, float th)
        :origin{0.0f, 0.0f}, basis(hex_basis(size, th)) {}

    Grid::Grid(float size, HexType T)
        :origin{0.0f, 0.0f},
        basis(hex_basis(size, T == HexType::POINTED? 0.0f : PI/6.0f)) {}

    Point Grid::hex_to_cartesian(const Hex& h) const noexcept
    {
        valarray<float> v{h.q, h.r};
        auto s = valarray<float>(basis[row(0)]*v);
        float x = accumulate(begin(s), end(s), 0.0f);
        s = valarray<float>(basis[row(1)]*v);
        float y = accumulate(begin(s), end(s), 0.0f);
        return Point{x+origin[0], y+origin[1]};
    }
}
