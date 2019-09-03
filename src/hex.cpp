#include "hex.hpp"
#include <cmath>        // abs, sqrt
#include <ostream>      // ostream
#include <algorithm>    // max_element, transform
#include <numeric>      // accumulate
#include <valarray>

using namespace std;

const float EPS = 0.01f;

namespace hexes {
    Hex::Hex() noexcept :q(0.0f), r(0.0f), s(0.0f) {}

    Hex::Hex(float q_, float r_) noexcept
        :q(q_), r(r_), s(-q_-r_) {}
    
    Hex::Hex(float q_, float r_, float s_)
        :q(q_), r(r_), s(s_)
    {
        if (abs(q+r+s) > EPS) {
            throw "q + r + s must = 0";
        }
    }
    
    float Hex::magnitude() const noexcept
    {
        return sqrt(q*q + r*r + s*s);
    }
    
    Hex operator-(const Hex& lhs, const Hex& rhs)
    {
        return Hex(lhs.q-rhs.q, lhs.r-rhs.r);
    }
    
    ostream& operator<<(ostream& out, const Hex& h)
    {
        out << "{" << h.q << ", " << h.r << ", " << h.s << "}";
        return out;
    }
}

namespace std {
    hexes::Hex round(const hexes::Hex& h)
    {
        // convert hex to valarray for easy operations
        valarray<float> v{h.q, h.r, h.s};

        // round each component
        auto rv = v;
        transform(begin(v), end(v), begin(rv),
                  [](float& e) { return round(e); });

        valarray<float> dv(abs(rv-v)); // difference between original and rounded

        // find the max difference and correct it
        auto i = distance(begin(dv), max_element(begin(dv), end(dv)));
        rv[i] -= accumulate(begin(rv), end(rv), 0.0f);

        return hexes::Hex(rv[0], rv[1], rv[2]);
    }
}
