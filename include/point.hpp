#ifndef HEXES_POINT_H
#define HEXES_POINT_H

#include <ostream>

namespace hax {
    struct Point {
        float x, y;
        Point(float x, float y);
        float magnitude() const noexcept;
    };

    Point operator-(const Point&, const Point&);
    std::ostream& operator<<(std::ostream&, const Point&);
}

#endif
