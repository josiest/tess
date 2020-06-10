#ifndef HEXES_HEX_HPP
#define HEXES_HEX_HPP

#include <iostream>  // ostream

namespace hax {

struct Hex {
    // TODO: make private and create getters:
    //  hex class should be immutable and prevent rep exposure
    /** The components of the Hex */
    float q, r, s;

    // TODO delete: prefer zero constant value
    /*** Create an empty Hex coordinate ({0, 0, 0}) */
    explicit Hex() noexcept;

    /** Create a Hex from axial coordinates.
     *
     * \param q the first component of the axial coordinate.
     * \param r the second component of the axial coordinate.
     */
    Hex(float q, float r) noexcept;

    /** Create a Hex from cube coordinates.
     *
     * \param q the first component of the cube coordinate.
     * \param r the second component of the cube coordinate.
     * \param s the third component of the cube coordinate.
     *
     * \throws std::invalid_argument if q + r + s != 0
     */
    Hex(float q, float r, float s);

    /**
     * The magnitude of this hex as if it were a euclidean 3d vector
     *
     * \return a non-negative number representing the magnitude of the
     *         three cube components in euclidean norm.
     */
    float magnitude() const noexcept;
};

// TODO: document?
bool operator==(const Hex&, const Hex&);
Hex operator-(const Hex&, const Hex&);
std::ostream& operator<<(std::ostream&, const Hex&);

// TODO: better names? also definitely nest inside hex class as static fields
inline static const Hex zero(0.0f, 0.0f);
inline static const Hex right_back(1.0f, -1.0f);
inline static const Hex up_back(0.0f, -1.0f);
inline static const Hex up_left(-1.0f, 0.0f);
inline static const Hex forward_left(-1.0f, 1.0f);
inline static const Hex forward_down(0.0f, 1.0f);
inline static const Hex right_down(1.0f, 0.0f);
}

namespace std {

// TODO: move to hax namespace: documentation is easier?
hax::Hex round(const hax::Hex&);

template <> struct hash<hax::Hex> {
    size_t operator()(const hax::Hex& h) const {
        hash<float> float_hash;
        size_t hq = float_hash(h.q);
        size_t hr = float_hash(h.r);
        return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
};
}

#endif
