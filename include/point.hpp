#ifndef HEXES_POINT_H
#define HEXES_POINT_H

#include <ostream>

namespace hax {

class Point {
    float const _x;
    float const _y;

public:
    /**
     * Create a representation of a cartesian point at (x, y)
     *
     * \param x the x-component of the cartesian point.
     * \param y the y-component of the cartesian point.
     */
    Point(float x, float y);

    /**
     * Get the x-component.
     *
     * \return the x-component of this point.
     */
    inline float x() const noexcept { return _x; }

    /**
     * Get the y-component.
     *
     * \return the y-component of this point.
     */
    inline float y() const noexcept { return _y; }
};

/**
 * Calculate the norm of p.
 *
 * \return the 2D euclidean norm of p.
 */
float norm(const Point& p) noexcept;

// TODO: document?
Point operator-(const Point&, const Point&);
std::ostream& operator<<(std::ostream&, const Point&);

}

#endif
