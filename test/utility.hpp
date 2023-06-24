#pragma once
#include "point.hpp"
#include <ostream>

template<tess::numeric_field Field>
std::ostream& operator<<(std::ostream& os, const tess::basic_point<Field>& p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
