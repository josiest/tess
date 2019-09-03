#include "math.hpp"
#include <valarray> // valarray
#include <cmath>    // cos, sin
#include <numeric>  // inner_product

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

    valarray<float> hex_basis(float size, float th)
    {
        if (size <= 0.0f) {
            throw "size must be > 0";
        }
        const float l = 2.0f*sin(PI/3.0f);
        valarray<float> basis(4);
        basis[col(0)] = angle_to_vec(th);
        basis[col(1)] = angle_to_vec(th+PI/3.0f);
        basis *= size*l;
        return basis;
    }

    valarray<float> inv_hex_basis(float size, float th)
    {
        if (size <= 0.0f) {
            throw "size must be > 0";
        }
        valarray<float> basis(4);
        basis[0] = (sqrt(3.0f)*cos(th) + sin(th))/2.0f;
        basis[1] = -sin(PI/6.0f - th);
        basis[2] = -sin(th);
        basis[3] = cos(th);
        basis *= 2.0f/(3.0f*size);
        return basis;
    }

    valarray<float> matvec_mul(const valarray<float>& mat,
                               const valarray<float>& vec)
    {
        valarray<float> row0(mat[row(0)]);
        valarray<float> row1(mat[row(1)]);

        valarray<float> product(2);
        product[0] = inner_product(begin(row0), end(row0), begin(vec), 0.0f);
        product[1] = inner_product(begin(row1), end(row1), begin(vec), 0.0f);

        return product;
    }
}
