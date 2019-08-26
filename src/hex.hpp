#ifndef HEXES_HEX_H
#define HEXES_HEX_H

#include <iostream>  // ostream

namespace hexes {
    struct Hex {
        /** The components of the Hex */
        float q, r, s;

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

    Hex operator-(const Hex&, const Hex&);
    std::ostream& operator<<(std::ostream&, const Hex&);
}

#endif
