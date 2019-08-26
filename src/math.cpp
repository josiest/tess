#include "math.hpp"
#include <valarray> // valarray
#include <cmath>    // cos, sin

using namespace std;

namespace hexes {
    valarray<float> angle_to_vec(float angle) noexcept
    {
        return valarray<float>{cos(angle), sin(angle)};
    }

    slice row(int i)
    {
        if (i < 0 || i > 1) {
            throw "i in row(i) must be either 0 or 1";
        }
        return slice(2*i, 2, 1);
    }

    slice col(int i)
    {
        if (i < 0 || i > 1) {
            throw "i in col(i) must be either 0 or 1";
        }
        return slice(i, 2, 2);
    }

    //valarray<float> hex_basis(float size, float th)
    //{
    //    const float l = 2.0f*sin(PI/3.0f);
    //    valarray<float> basis(4);
    //    basis[row(0)] = angle_to_vec(th);
    //    basis[row(1)] = angle_to_vec(th+PI/6.0f);
    //    basis *= size*l;
    //    return basis;
    //}
}
