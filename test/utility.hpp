#pragma once
#include "point.hpp"
#include "hex.hpp"
#include <ostream>

template<tess::numeric_field Field>
std::ostream& operator<<(std::ostream& os, const tess::basic_point<Field>& p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

template<tess::numeric_field Field>
std::ostream& operator<<(std::ostream& os, const tess::basic_hex<Field>& h)
{
    os << "(" << h.q << ", " << h.r << ", " << h.s() << ")";
    return os;
}
