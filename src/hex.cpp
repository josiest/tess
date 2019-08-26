#include "hex.hpp"
#include <cmath>    // abs, sqrt
#include <ostream>  // ostream

using namespace std;

const float EPS = 0.01f;

namespace hexes {
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
