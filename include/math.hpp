#ifndef HEXES_MATH_H
#define HEXES_MATH_H

#include <cmath>    // acos
#include <valarray> // valarray, slice

namespace hexes {

    /** pi constant */
    const float PI = std::acos(-1.0f);

    /** Convert an angle in radians to its 2D unit vector equivalent
     */
    std::valarray<float> angle_to_vec(float) noexcept;
}

#endif
