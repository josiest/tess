#include "point.hpp"
#include <cmath>    // sqrt

using namespace std;

namespace hax {

Point::Point(float x, float y) :_x(x), _y(y) {}

float norm(const Point& p) noexcept
{
    return sqrt(p.x()*p.x() + p.y()*p.y());
}

Point operator-(const Point& lhs, const Point& rhs)
{
    return Point{lhs.x()-rhs.x(), lhs.y()-rhs.y()};
}

ostream& operator<<(ostream& out, const Point& p)
{
    out << "{" << p.x() << ", " << p.y() << "}";
    return out;
}

}
