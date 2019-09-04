#ifndef HEXES_HEX_HPP
#define HEXES_HEX_HPP

#include <iostream>  // ostream

namespace hexes {
    struct Hex {
        /** The components of the Hex */
        float q, r, s;

        /** Create an empty Hex coordinate ({0, 0, 0})
         */
        explicit Hex() noexcept;

        /** Create a Hex from axial coordinates.
         *
         * Parameters:
         *  q, r - the components of the axial coordinate
         */
        Hex(float, float) noexcept;

        /** Create a Hex from cube coordinates.
         *
         * Parameters:
         *  q, r, s - the components of the cube coordinate
         *
         * Exceptions:
         *  throws error if q + r + s != 0
         */
        Hex(float, float, float);

        /** The magnitude of this Hex as if it were a euclidean 3d vector
         */
        float magnitude() const noexcept;
    };

    bool operator==(const Hex&, const Hex&);

    Hex operator-(const Hex&, const Hex&);
    std::ostream& operator<<(std::ostream&, const Hex&);

    inline static const Hex zero(0.0f, 0.0f);
    inline static const Hex right_back(1.0f, -1.0f);
    inline static const Hex up_back(0.0f, -1.0f);
    inline static const Hex up_left(-1.0f, 0.0f);
    inline static const Hex forward_left(-1.0f, 1.0f);
    inline static const Hex forward_down(0.0f, 1.0f);
    inline static const Hex right_down(1.0f, 0.0f);
}

namespace std {
    hexes::Hex round(const hexes::Hex&);
    
    template <> struct hash<hexes::Hex> {
        size_t operator()(const hexes::Hex& h) const {
            hash<float> float_hash;
            size_t hq = float_hash(h.q);
            size_t hr = float_hash(h.r);
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };
}

#endif
