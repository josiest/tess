#ifndef HEXES_GRID_HPP
#define HEXES_GRID_HPP

#include "hex.hpp"
#include "point.hpp"
#include <valarray>
#include <vector>
#include <unordered_set>

namespace hax {

    enum class HexType { Pointed, Flat };

    /** Interface to a hexagonal grid.
     */
    class Grid {
        std::valarray<float> _origin;
        float _unit_size;
        float _offset;
        std::valarray<float> basis;
        std::valarray<float> inv_basis;
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

        /** Get or set the unit size of the grid.
         *
         * Parameters:
         *  size - of the grid; must be positive
         *
         * Getting the unit_size returns a float.
         *
         * Throws:
         *  error if size <= 0
         */
        float unit_size() const noexcept;
        void unit_size(float);

        /** Get or set the offset angle of the grid.
         *
         * Parameters
         *  th - the offset angle
         *
         * Getting the offset returns a float.
         */
        float offset() const noexcept;
        void offset(float) noexcept;

        /** Get or set the origin of the grid.
         *
         * Parameters:
         *
         * origin(float x, float y)
         *  x, y - the new origin
         *
         * origin(const Point& p)
         *  p - the new origin
         *
         * Getting the offset returns a Point object.
         */
        Point origin() const noexcept;
        void origin(float, float) noexcept;
        void origin(const Point&) noexcept;

        /** Convert a hex coordinate to cartesian (in global space)
         *
         * Parameters:
         *  h - the hex coordinate to convert
         */
        Point hex_to_cartesian(const Hex&) const noexcept;

        /** Convert a cartesian coordinate (in global space) to a hex coordinate
         *
         * Parameters:
         *  x, y - the coordinate to convert
         */
        Hex cartesian_to_hex(float x, float y) const noexcept;

        /** Convert a cartesian coordinate (in global space) to a hex coordinate
         *
         * Parameters:
         *  p - the coordinate to convert
         */
        Hex cartesian_to_hex(const Point& p) const noexcept;

        /** Give the cartesian coordinates of each vertex of a hex.
         *
         * Parameters:
         *  h - the hex coordinate to get the vertices of
         */
        std::vector<Point> vertices(const Hex&) const noexcept;

        /** Create a hexagonal map of hexes.
         *
         * Parameters:
         *  radius - the radius of the map.
         *
         * Throws:
         *  error if radius is less than zero.
         */
        static std::unordered_set<Hex> hex_map(int);
    };
}

#endif
