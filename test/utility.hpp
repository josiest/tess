#pragma once
#include "fields.hpp"
#include "point.hpp"
#include "hex.hpp"
#include <ostream>

namespace tess {
template<tess::numeric Field>
void PrintTo(const tess::point<Field>& p, std::ostream* os)
{
    *os << "(" << p.x << ", " << p.y << ")";
}

template<tess::numeric Field>
void PrintTo(const tess::hex<Field>& h, std::ostream* os)
{
    *os << "(" << h.q << ", " << h.r << ", " << h.s() << ")";
}
}
