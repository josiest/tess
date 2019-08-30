#ifndef HEXES_GRID_H
#define HEXES_GRID_H

#include "hex.hpp"      // Hex
#include "point.hpp"    // Point
#include <valarray>     // valarray
#include <vector>       // vector

namespace hexes {

    enum class HexType { Pointed, Flat };

    /** Interface to a hexagonal grid.
     */
    class Grid {
        std::valarray<float> origin;
        float unit_size;
        float offset;
        std::valarray<float> basis;
    public:
        /** Construct a grid with a given origin, unit size and offset angle.
         *
         * Parameters:
         *  x, y - the cartesian origin of the grid (in global space)
         *  size - the unit size of the grid
         *  th   - the offset angle of the grid
         *
         * Throws:
         *  error if size <= 0
         */
        Grid(float, float, float, float);

        /** Construct a grid with a given unit size and offset angle.
         *
         * Parameters:
         *  size - the unit size of the grid
         *  th   - the offset angle of the grid
         *
         * Throws:
         *  error if size <= 0
         */
        Grid(float, float);

        /** Construct a grid with a given unit size and hex type.
         *
         * Parameters:
         *  size    - the unit size of the grid
         *  hextype - the type of hex unit
         *
         * Throws:
         *  error if size <= 0
         */
        Grid(float, HexType);

        /** Convert a hex coordinate to cartesian (in global space)
         *
         * Parameters:
         *  h - the hex coordinate to convert
         */
        Point hex_to_cartesian(const Hex&) const noexcept;

        /** Give the cartesian coordinates of each vertex of a hex.
         *
         * Parameters:
         *  h - the hex coordinate to get the vertices of
         */
        std::vector<Point> vertices(const Hex&) const noexcept;
    };
}

#endif
