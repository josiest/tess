#include "math.hpp"
#include <valarray> // valarray
#include <cmath>    // cos, sin

using namespace std;

namespace hexes {
    valarray<float> angle_to_vec(float angle) noexcept
    {
        return valarray<float>{cos(angle), sin(angle)};
    }
}
