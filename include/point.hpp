#ifndef HEXES_POINT_H
#define HEXES_POINT_H

#include <ostream>

namespace hax {

// TODO: make field type a generic type
struct Point {
    // TODO: make private:
    //  Point should be immutable and prevent rep exposure
    float x, y;

    /**
     * Create a representation of a cartesian point at (x, y)
     *
     * \param x the x-component of the cartesian point.
     * \param y the y-component of the cartesian point.
     */
    Point(float x, float y);
    // TODO: rename to norm, or overload std::abs
    /**
     * Calculate the norm of this point.
     *
     * \return the 2D euclidean norm of this point.
     */
    float magnitude() const noexcept;
};

// TODO: document?
Point operator-(const Point&, const Point&);
std::ostream& operator<<(std::ostream&, const Point&);

}

#endif
