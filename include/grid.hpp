#ifndef HEXES_GRID_HPP
#define HEXES_GRID_HPP

#include "hex.hpp"
#include "point.hpp"
#include <valarray>
#include <vector>
#include <unordered_set>

namespace hax {

enum class HexType { Pointed, Flat };

/**
 * Interface to a hexagonal grid.
 */
class Grid {
    std::valarray<float> _origin;
    float _unit_size;
    float _offset;
    std::valarray<float> basis;
    std::valarray<float> inv_basis;
public:
    // TODO: redesign constructors to be more uniform

    /**
     * Construct a grid with a global origin, unit size and offset angle.
     *
     * \param x the x-coordinate of the cartesian origin.
     * \param y the y-coordinate of the cartesian origin.
     * \param size the unit size of the grid, a positive number.
     * \param th the offset angle of the grid in radians.
     *
     * \throws std::invalid_argument if size <= 0.
     */
    Grid(float, float, float, float);

    /**
     * Construct a grid with a given unit size and offset angle.
     *
     * \param size the unit size of the grid, a positive number.
     * \param th the offset angle of the grid in radians.
     *
     * \throws std::invalid_argument if size <= 0.
     */
    Grid(float, float);

    /**
     * Construct a grid with a given unit size and hex type.
     *
     * \param size the unit size of the grid, a positive number.
     * \param hextype the offset angle of the grid as a HexType
     *
     * \throws std::invalid_argument if size <= 0.
     */
    Grid(float, HexType);

    /**
     * Get the unit size of this grid.
     *
     * \return a positive number represing this grid's unit size.
     */
    float unit_size() const noexcept;

    // TODO: grid class should be immutable
    /**
     * Set the unit size of this grid
     *
     * \param size the unit size of the grid, a positive number.
     * \post change this grid's unit size to size.
     * \throws std::invalid_argument if size <= 0.
     */
    void unit_size(float);

    /**
     * Get the offset angle of this grid.
     *
     * \return the offset angle of this grid in radians.
     */
    float offset() const noexcept;

    // TODO: delete: grid class should be immutable
    /**
     * Set the offset angle of this grid.
     *
     * \param theta the offset angle in radians.
     * \post change this grid's offset angle to theta.
     */
    void offset(float) noexcept;

    /**
     * Get the global origin of this grid.
     *
     * \return the cartesian origin of this grid in global space.
     */
    Point origin() const noexcept;

    // TODO: delete: Grid class should be immutable
    /**
     * Set the origin of this grid.
     *
     * \param x the x-component of the cartesian origin.
     * \param y the y-component of the cartesian origin.
     * \post change this grid's origin to (x, y)
     */
    void origin(float, float) noexcept;

    // TODO: delete: Grid class should be immutable
    /**
     * Set the origin of this grid.
     *
     * \param p the cartesian origin.
     * \post change this grid's origin to p
     */
    void origin(const Point&) noexcept;

    /**
     * Convert a hex coordinate to cartesian.
     *
     * \param h the hex coordinate to convert.
     * \return the cartesian representation of the hex coordinate
     *         with respect to global origin.
     */
    Point hex_to_cartesian(const Hex&) const noexcept;

    /**
     * Convert a cartesian coordinate to a hex coordinate
     *
     * \param x the x-component of the cartesian coordinate in global space.
     * \param y the y-component of the cartesian coordinate in global space.
     *
     * \return the hex representation of (x, y) with respect to this grid's
     *         origin.
     */
    Hex cartesian_to_hex(float x, float y) const noexcept;

    /**
     * Convert a cartesian coordinate to a hex coordinate.
     *
     * \param p the cartesian coordinate in global space.
     * \return the hex representation of p with respect to this grid's origin.
     */
    Hex cartesian_to_hex(const Point& p) const noexcept;

    /**
     * Calculate the cartesian coordinates of each vertex of a hex.
     *
     * \param h the hex coordinate to calculate the vertices of.
     * \return a list of cartesian coordinates with respect to global origin
     *         representing the vertices of this hex coordinate. Vertices appear
     *         in clockwise order starting from upper-right-most vertex.
     */
    std::vector<Point> vertices(const Hex&) const noexcept;

    // TODO: delete: a pointless function? or at least rename
    /**
     * Create a hexagonal map of hexes.
     *
     * \param r the radius in hex units of the map.
     * \return a set of all hex coordinates in the map.
     * \throws std::illegal_argument if r < 0
     */
    static std::unordered_set<Hex> hex_map(int);
};
}

#endif
