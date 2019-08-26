#include "hex.hpp"
#include "math.hpp"
#include <iostream>     // ostream, cout
#include <cmath>        // sqrt
#include <vector>       // vector
#include <functional>   // function
#include <array>        // array
#include <valarray>     // valarray, abs(valarray)
#include <sstream>      // stringstream

using namespace hexes;
using namespace std::literals::string_literals;
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

string vecstr(valarray<float> vec)
{
    stringstream ss;
    ss << "{";
    auto sep = "";
    for (auto x : vec) {
        ss << sep << x;
        sep = ", ";
    }
    ss << "}";
    return ss.str();
}

void test_row_and_col(ostream& out)
{
    out << "Testing row() and col()\n";

    // testing row and col with good input
    using slicefunc = function<slice(int)>;
    array<slicefunc, 2> funcs{row, col};
    array<string, 2> names{"row"s, "col"s};
    array<int, 2> strides{1, 2};
    array<int, 4> starts{0, 2, 0, 1};

    bool failed = false;

    for (int k = 0; k < 4; ++k) {
        const int i = k % 2;
        const int j = k / 2;

        auto name = names[j];
        auto func = funcs[j];

        slice actual;
        try {
            actual = func(i);
        }
        catch (...) {
            failed = true;
            out << "\t" << name << "(" << i << ") threw an error; expected "
                << "slice(" << starts[k] << ", 2, " << strides[j] << ")\n";
            continue;
        }
        bool starts_equal = actual.start() == size_t(starts[k]);
        bool sizes_equal = actual.size() == size_t(2);
        bool strides_equal = actual.stride() == size_t(strides[j]);

        if (!starts_equal || !sizes_equal || !strides_equal) {
            failed = true;
            out << "\t" << name << "(" << i << ") gave slice("
                << actual.start() << ", " << actual.size() << ", "
                << actual.stride() << "); expected slice(" << starts[k]
                << ", 2, " << strides[j] << ")\n";
        }
    }

    // testing row and col with bad input
    array<int, 2> bad_indices{-1, 2};
    for (int k = 0; k < 4; ++k) {

        const int i = k % 2;
        const int j = k / 2;

        auto name = names[j];
        auto func = funcs[j];

        bool caught = false;
        slice actual;
        try {
            actual = func(bad_indices[i]);
        }
        catch (...) {
            caught = true;
        }
        if (!caught) {
            failed = true;
            out << "\t" << name << "(" << i << ") gave slice("
                << actual.start() << ", " << actual.size() << ", "
                << actual.stride() << "); excpected an exception\n";
        }
    }

    // Wrap up
    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

/*void test_hex_basis(ostream& out)
 *{
 *    const float num_tests = 3;
 *    array<float, num_tests> sizes{
 *        0.3f,    1.0f,  5.3f
 *    };
 *    array<float, num_tests> angles{
 *        0.0f, PI/6.0f, 33.4f
 *    };
 *
 *    array<valarray<float>, num_tests> expected_vecs{
 *        {           0.3f,     0.15f,     0.0f, 0.259808f},
 *        {sqrt(3.0f)/2.0f,      0.0f,     0.0f,      1.0f},
 *        {      -2.12855f, -5.26778f, 4.85379f, 0.583519f}
 *    };
 *    for (auto vec : expected_vecs) {
 *        vec *= 2.0f*sin(PI/3.0f);
 *    }
 *
 *    out << "Testing hex_basis\n";
 *
 *    bool failed = false;
 *    bool caught = false;
 *    for (int i = 0; i < num_tests; ++i) {
 *        auto size = size[i];
 *        auto th = angles[i];
 *        auto expected = expected_vecs[i];
 *
 *        auto actual = hex_basis(size, th);
 *
 *        if (abs(actual-expected) > EPS) {
 *            failed = true;
 *            out << "hex_basis(" << size << ", " << th << ") gave a basis of "
 *                << vecstr(actual) << "; expected " << vectstr(expected)
 *                << "\n";
 *        }
 *    }
 *}
 */

int main()
{
    vector<test_func> tests{
        test_Hex, test_angle_to_vec, test_row_and_col
    };

    for (auto test : tests) {
        test(cout);
    }
}
