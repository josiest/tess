#include "hex.hpp"
#include "math.hpp"
#include <iostream>     // ostream, cout
#include <cmath>        // sqrt
#include <vector>       // vector
#include <functional>   // function
#include <array>        // array

using namespace hexes;
using namespace std;

using test_func = function<void(ostream&)>;

const float EPS = 0.01f;

// test Hex constructors, as well as subtraction and magnitude
void test_Hex(ostream& out)
{
    bool failed = false;
    out << "Testing Hex constructors\n";

    // Test Axial constructor
    Hex actual(3.0f, 4.0f);
    Hex expected{3.0f, 4.0f, -7.0f};

    if ((actual-expected).magnitude() > EPS) {
        failed = true;
        out << "\tHex(3.0f, 4.0f) gave a Hex coordinate of " << actual
            << "; expected " << expected << "\n";
    }

    // Test Cube constructor w/good input

    bool caught = false;
    expected = {1.1f, -3.3f, 2.2f};
    try {
        actual = Hex(1.1f, -3.3f, 2.2f);
    }
    catch (...) {
        failed = true;
        caught = true;
        out << "\tHex(1.1f, -3.3f, 2.2f) threw an exception; expected "
            << expected << "\n";
    }
    if (!caught && (actual-expected).magnitude() > EPS) {
        failed = true;
        out << "\tHex(1.1f, -3.3f, 2.2f) gave a Hex coordinate of " << actual
            << "; expected " << expected << "\n";
    }

    // Test Cube constructor w/bad input
    caught = false;

    try {
        actual = Hex(39.9f, 22.3f, -1.1f);
    }
    catch (...) {
        caught = true;
    }
    if (!caught) {
        failed = true;
        out << "\tHex(39.9f, 22.3f, -1.1f) gave a Hex coordinate of " << actual
            << "; expected an exception\n";
    }

    // Wrap up
    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

void test_angle_to_vec(ostream& out)
{
    out << "Testing angle_to_vec\n";
    const int size = 6;
    array<float, size> angles{
        0.0f,         PI/3.0f, PI/2.0f,    PI, 4.1f/sqrt(3.0f), 3.0f*PI/2.0f
    };
    array<float, size> xcomp{
        1.0f,            0.5f,    0.0f, -1.0f,      -0.714801f,         0.0f
    };
    array<float, size> ycomp{
        0.0f, sqrt(3.0f)/2.0f,    1.0f,  0.0f,       0.699328f,        -1.0f
    };
    bool failed = false;

    for (int i = 0; i < size; ++i) {
        auto th = angles[i];
        auto x = xcomp[i];
        auto y = ycomp[i];

        auto v = angle_to_vec(th);
        if ((abs(v[0]-x) > EPS) || (abs(v[1]-y) > EPS)) {
            failed = true;
            out << "\tangle_to_vec(" << th << ") gave {" << v[0] << ", "
                << v[1] << "}; expected {" << x << ", " << y << "}\n";
        }
    }
    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

int main()
{
    vector<test_func> tests{
        test_Hex, test_angle_to_vec
    };

    for (auto test : tests) {
        test(cout);
    }
}
