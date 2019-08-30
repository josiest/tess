#include "hex.hpp"
#include "math.hpp"
#include "grid.hpp"
#include "point.hpp"
#include <iostream>     // ostream, cout
#include <cmath>        // sqrt
#include <vector>       // vector
#include <functional>   // function
#include <array>        // array
#include <valarray>     // {valarray, abs(valarray), begin(valarray),
                        //  end(valarray)}
#include <sstream>      // stringstream
#include <numeric>      // inner_product

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

void test_hex_basis(ostream& out)
{
    // testing hex_basis with good values

    constexpr size_t num_tests = 5;
    array<float, num_tests> sizes{
        0.3f,    1.0f,  5.3f,          100.0f,  15.0f
    };
    array<float, num_tests> angles{
        0.0f, PI/6.0f, 33.4f, -PI/sqrt(47.0f), -77.2f
    };

    array<valarray<float>, num_tests> expected_vecs{
        valarray<float>{           0.3f,     0.15f,      0.0f, 0.259808f},
        valarray<float>{sqrt(3.0f)/2.0f,      0.0f,      0.5f,      1.0f},
        valarray<float>{      -2.12855f, -5.26778f,  4.85379f, 0.583519f},
        valarray<float>{       89.6829f,  83.1525f, -44.2368f,  55.5488f},
        valarray<float>{      -3.43397f,  10.9284f, -14.6016f, -10.2747f}
    };
    for (auto& vec : expected_vecs) {
        vec *= 2.0f*sin(PI/3.0f);
    }
    out << "Testing hex_basis\n";
    if (abs(expected_vecs[0][0]-0.3f) <= 0.01f) {
        out << "\texpected_vecs weren't mutated!\n";
        return;
    }

    bool failed = false;
    for (size_t i = 0; i < num_tests; ++i) {
        auto size = sizes[i];
        auto th = angles[i];
        auto expected = expected_vecs[i];

        valarray<float> actual;
        try {
            actual = hex_basis(size, th);
        }
        catch (...) {
            failed = true;
            out << "\thex_basis(" << size << ", " << th << ") threw an "
                << "exception; excpected " << vecstr(expected) << "\n";
            continue;
        }

        valarray<float> diff = abs(actual-expected);
        auto mag = inner_product(begin(diff), end(diff), begin(diff), 0.0f);
        if (mag > EPS) {
            failed = true;
            out << "\thex_basis(" << size << ", " << th << ") gave a basis of "
                << vecstr(actual) << "; expected " << vecstr(expected)
                << "\n";
        }
    }

    // test hex_basis for bad input

    array<float, 2> bad_sizes{0.0f, -2.1f};

    for (int i = 0; i < 2; ++i) {

        bool caught = false;
        valarray<float> actual;
        try {
            actual = hex_basis(bad_sizes[i], angles[i]);
        }
        catch (...) {
            caught = true;
        }
        if (!caught) {
            failed = true;
            out << "\thex_basis" << bad_sizes[i] << ", " << angles[i]
                << ") gave a basis of " << vecstr(actual) << "; expected an "
                << "exception\n";
        }
    }

    // Wrap up
    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

void test_Grid(ostream& out)
{
    // testing grid with bad input
    out << "Testing grid constructors\n";
    bool failed = false;
    bool caught = false;
    try {
        Grid(-1.0f, HexType::Flat);
    }
    catch (...) {
        caught = true;
    }
    if (!caught) {
        failed = true;
        out << "\tGrid() didn't throw an exception\n";
    }

    caught = false;
    try {
        Grid(-0.0001f, -0.0001f);
    }
    catch (...) {
        caught = true;
    }
    if (!caught) {
        failed = true;
        out << "\tGrid() didn't throw an exception\n";
    }

    caught = false;
    try {
        Grid(1.0f, -1.0f, -13.33f, 0.25f);
    }
    catch (...) {
        caught = true;
    }
    if (!caught) {
        failed = true;
        out << "\tGrid() didn't throw an exception\n";
    }

    // Wrap up
    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

void test_hex_to_cartesian(ostream& out)
{
    // testing hex_to_cartesian with good input
    constexpr int num_tests = 4;
    array<Grid, num_tests> grids{
        Grid(1.0f, HexType::Pointed), Grid(0.21f, HexType::Flat),
        Grid(3.87f, PI/sqrt(71.0f)), Grid(73.0f, -13.4f, 15.0f, -77.2f)
    };
    array<Hex, num_tests> hexes{
        Hex(0.0f, 0.0f), Hex(0.5f, -0.5f),
        Hex(32.3f, 17.4f), Hex(-20.2f, -0.13f)
    };
    array<Point, num_tests> expected_points{
        Point{0.0f, 0.0f}, Point{0.1575f, -0.0909327f},
        Point{219.15f, 194.175f}, Point{190.685f, 499.787}
    };

    out << "Testing grid.hex_to_cartesian\n";

    bool failed = false;
    for (int i = 0; i < num_tests; ++i) {
        auto expected = expected_points[i];
        auto actual = grids[i].hex_to_cartesian(hexes[i]);

        if ((actual-expected).magnitude() > EPS) {
            failed = true;
            out << "\tGrid converted hex coordinate " << hexes[i]
                << " to cartesian coordinate " << actual << "; expected "
                << expected << "\n";
        }
    }

    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

void test_vertices(ostream& out)
{
    constexpr int num_tests = 4;
    array<Grid, num_tests> grids{
        Grid(1.0f, HexType::Pointed), Grid(13.13f, HexType::Flat),
        Grid(20.0f, 4.0f*PI/5.0f), Grid(90.0f, 70.0f, 0.11f, 413.33f)
    };
    array<Hex, num_tests> hexes{
        Hex(0.0f, 0.0f), Hex(1.0f, 1.0f), Hex(-0.3f, 74.5f), Hex(-13.0f, -2.0f)
    };
    array<vector<Point>, num_tests> expected_vertices{
        vector<Point>{
            Point(sqrt(3.0f)/2.0f, 0.5f), Point(0.0f, 1.0f),
            Point(-sqrt(3.0f)/2.0f, 0.5f), Point(-sqrt(3.0f)/2.0f, -0.5f),
            Point(0.0f, -1.0f), Point(sqrt(3.0f)/2.0f, -0.5f)
        },
        vector<Point>{
            Point(26.26f, 45.4837f), Point(13.13f, 45.4837f),
            Point(6.565f, 34.1127f), Point(13.13f, 22.7418f),
            Point(26.26f, 22.7418f), Point(32.825f, 34.1127f)
        },
        vector<Point>{
            Point(-2369.12f, -1053.71f), Point(-2360.99f, -1071.98f),
            Point(-2341.1f, -1074.07f), Point(-2329.34f, -1057.89f),
            Point(-2337.47f, -1039.62f), Point(-2357.36f, -1037.53f)
        },
        vector<Point>{
            Point(89.1935f, 72.4578f), Point(89.2273f, 72.5625f),
            Point(89.1536f, 72.6441f), Point(89.0461f, 72.6211f),
            Point(89.0122f, 72.5165f), Point(89.0859f, 72.4348f)
        }
    };

    out << "Testing grid.vertices\n";
    bool failed = false;

    for (int i = 0; i < num_tests; ++i) {
        auto actual = grids[i].vertices(hexes[i]);
        auto expected = expected_vertices[i];

        for (size_t j = 0; j < actual.size(); ++j) {
            if ((actual[j]-expected[j]).magnitude() > EPS) {
                failed = true;
                out << "\tComponent " << j << " in grid.vertices(" << hexes[i]
                    << ") is " << actual[j] << "; expected " << expected[j]
                    << "\n";
            }
        }
    }

    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

int main()
{
    vector<test_func> tests{
        test_Hex, test_angle_to_vec, test_row_and_col, test_hex_basis,
        test_Grid, test_hex_to_cartesian, test_vertices
    };

    for (auto test : tests) {
        test(cout);
    }
}
