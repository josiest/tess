#ifndef HEXES_MATH_HPP
#define HEXES_MATH_HPP

#include <cmath>    // acos
#include <valarray> // valarray, slice

namespace hax {

    /** pi constant */
    const float PI = std::acos(-1.0f);

    /** Convert an angle in radians to its 2D unit vector equivalent
     *
     * Parametetrs:
     *  angle - the angle in radians to compose from
     */
    std::valarray<float> angle_to_vec(float) noexcept;

    /** Row i of a 2x2 matrix
     *
     * Parameters:
     *  i - the index of the row (0 or 1)
     */
    std::slice row(int);

    /** Column i of a 2x2 matrix
     *
     * Parameters:
     *  i - the index of the column (0 or 1)
     */
    std::slice col(int);

    /** Define the basis matrix for converting hex coordinates to cartesian.
     *
     * Parameters:
     *  size - the unit size of each hex
     *  th - the offset angle of the grid
     */
    std::valarray<float> hex_basis(float, float);

    /** Define the basis matrix for converting cartesian coordinates to hex.
     *
     * Parameters:
     *  size - the unit size of each hex
     *  th - the offset angle of the grid
     */
    std::valarray<float> inv_hex_basis(float, float);

    /** Perform matrix-vector multiplication on a 2x2 matrix and a 2x1 vector
     *
     * Paramters:
     *  mat - the 2x2 matrix
     *  vec - the 2x1 vector
     */
    std::valarray<float> matvec_mul(const std::valarray<float>&,
                                    const std::valarray<float>&);
}

#endif
