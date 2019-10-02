#include "hex.hpp"
#include "math.hpp"
#include "grid.hpp"
#include "point.hpp"

#include <vector>
#include <array>
#include <valarray>

#include <iostream>     // ostream, cout
#include <cmath>        // sqrt
#include <functional>   // function
#include <sstream>      // stringstream
#include <numeric>      // inner_product
#include <variant>      // variant, get, get_if
#include <utility>      // pair, make_pair

using namespace hax;
using namespace std::literals::string_literals;
using namespace std;

using test_func = function<void(ostream&)>;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

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

    constexpr size_t num_tests = 7;
    array<float, num_tests> sizes{
        0.3f,    1.0f,  5.3f,          100.0f,  15.0f,   23.0f,   20.0f
    };
    array<float, num_tests> angles{
        0.0f, PI/6.0f, 33.4f, -PI/sqrt(47.0f), -77.2f, PI/6.0f, PI/5.0f
    };

    array<valarray<float>, num_tests> expected_bases{
        valarray<float>{           0.3f,     0.15f,      0.0f, 0.259808f},
        valarray<float>{sqrt(3.0f)/2.0f,      0.0f,      0.5f,      1.0f},
        valarray<float>{      -2.12855f, -5.26778f,  4.85379f, 0.583519f},
        valarray<float>{       89.6829f,  83.1525f, -44.2368f,  55.5488f},
        valarray<float>{      -3.43397f,  10.9284f, -14.6016f, -10.2747f},
        valarray<float>{       19.9186f,      0.0f,     11.5f,     23.0f},
        valarray<float>{       16.1803f, -2.09057f,  11.7557f,  19.8904f}
    };
    for (auto& vec : expected_bases) {
        vec *= 2.0f*sin(PI/3.0f);
    }
    out << "Testing hex_basis\n";
    if (abs(expected_bases[0][0]-0.3f) <= 0.01f) {
        out << "\texpected_bases weren't mutated!\n";
        return;
    }

    array<valarray<float>, num_tests> expected_inv_bases{
        valarray<float>{    1.9245f,   -1.11111f,        0.0f,    2.22222f},
        valarray<float>{  0.666667f,        0.0f,  -0.333333f,    0.57735f},
        valarray<float>{ 0.0138488f,   0.125021f,  -0.115196f, -0.0505173f},
        valarray<float>{0.00370325f, -0.0055435f, 0.00294919f, 0.00597886f},
        valarray<float>{-0.0304436f, -0.0323805f,  0.0432641f, -0.0101747f},
        valarray<float>{ 0.0289855f,        0.0f, -0.0144928f,  0.0251022f},
        valarray<float>{ 0.0331507f, 0.00348428f, -0.0195928f,  0.0269672f}
    };

    out << "Testing inv_hex_basis\n";
    bool failed = false;
    for (size_t i = 0; i < num_tests; ++i) {
        auto size = sizes[i];
        auto th = angles[i];
        auto expected = expected_bases[i];

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

        expected = expected_inv_bases[i];
        try {
            actual = inv_hex_basis(size, th);
        }
        catch (...) {
            failed = true;
            out << "\tinv_hex_basis(" << size << ", " << th << ") threw an "
                << "exception; excpected " << vecstr(expected) << "\n";
            continue;
        }

        diff = abs(actual-expected);
        mag = inner_product(begin(diff), end(diff), begin(diff), 0.0f);
        if (mag > EPS) {
            failed = true;
            out << "\tinv_hex_basis(" << size << ", " << th << ") gave a basis "
                << "of " << vecstr(actual) << "; expected " << vecstr(expected)
                << "\n";
        }
    }

    // test hex_basis and inv_hex_basis for bad input

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
            out << "\thex_basis(" << bad_sizes[i] << ", " << angles[i]
                << ") gave a basis of " << vecstr(actual) << "; expected an "
                << "exception\n";
        }

        caught = false;
        try {
            actual = inv_hex_basis(bad_sizes[i], angles[i]);
        }
        catch (...) {
            caught = true;
        }
        if (!caught) {
            failed = true;
            out << "\tinv_hex_basis(" << bad_sizes[i] << ", " << angles[i]
                << ") gave a basis of " << vecstr(actual) << "; expected an "
                << "exception\n;";
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

void test_cartesian_to_hex(ostream& out)
{
    constexpr int num_tests = 12;
    array<Grid, num_tests> grids {
        Grid(1.0f, HexType::Pointed), Grid(23.0f, HexType::Flat),
        Grid(0.573f, 8.0f*PI/5.0f), Grid(20.0f, 17.14f, 5.39f, 2.3f*PI/3.3f),
        Grid(10.0f, PI/5.0f), Grid(-20.0f, 50.0f, 100.0f, 9.0f*PI/7.0f),
        Grid(10.0f, 10.0f, 3.0f, PI/2.0f), Grid(13.9, 3.0f*PI/7.0f),
        Grid(72.3, PI/4.0f), Grid(0.5, PI/13.0f), Grid(1, 11.0f*PI/12.0f),
        Grid(1.0f, HexType::Flat)
    };
    array<variant<pair<float, float>, Point>, num_tests> points {
        pair<float, float>{0.0f, 0.0f}, Point(-100.0f, 100.0f),
        pair<float, float>{-300.3f, -76.19f}, Point(13.0f, 15.0f),
        pair<float, float>{43.0f, -31.0f}, Point(758.0f, 304.0f),
        pair<float, float>{-17.4f, 29.3f}, Point(-70.0f, -80.0f),
        pair<float, float>{800.0f, 0.0f}, pair<float, float>{-13.0f, -26.0f},
        pair<float, float>{1.0f, -1.0f}, pair<float, float>{10.0f, 13.0f}
    };
    array<Hex, num_tests> expected {
        Hex(0.0f, 0.0f), Hex(-2.89855f, 3.95949f), Hex(159.35f, -359.682f),
        Hex(-0.18119f, 0.858794f), Hex(2.63494f, -3.35695f),
        Hex(-5.44678f, 2.99932f), Hex(0.669842f, 6.08889f),
        Hex(-5.09622f, 2.41934f), Hex(7.12532f, -5.21609f),
        Hex(-7.00411f, -29.5112f), Hex(-0.942809f, 0.471405f),
        Hex(6.66667f, 4.17222f)
    };

    out << "Testing grid.cartesian_to_hex\n";
    bool failed = false;

    for (int i = 0; i < num_tests; ++i) {
        Hex actual;
        stringstream ss;

        std::visit(overloaded {

            [&](Point& p) {
                actual = grids[i].cartesian_to_hex(p);
                ss << p;
            },

            [&](auto& p) {
                auto pa = get<pair<float, float>>(points[i]);
                actual = grids[i].cartesian_to_hex(pa.first, pa.second);
                ss << "{" << pa.first << ", " << pa.second << "}";
            }

        }, points[i]);

        if ((actual-expected[i]).magnitude() > EPS) {
            out << "\tgrid.cartesian_to_hex(" << ss.str() << ") gave a hex of"
                << actual << "; expected " << expected[i] << "\n";
            failed = true;
        }
    }
    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

void test_hex_round(ostream& out)
{
    constexpr int num_tests = 5;
    array<Hex, num_tests> hexes {
        Hex(0.31f, 0.28f),  // {x, y} near 0 at mid
        Hex(-32.2f, 28.3f), // {x<-1, y>1} ceiling
        Hex(-0.51f, 1.27f), // {x, y} near -1, floor
        // {x near 1: floor | y<-1: ceiling}
        Hex(1.13f, 1.86f), 
        // {x > 1: ceiling | y near 1: floor}
        Hex(703.6f, -704.85f),
    };
    array<Hex, num_tests> expected {
        Hex(0.0f, 0.0f),
        Hex(-32.0f, 28.0f),
        Hex(0.0f, 1.0f),
        Hex(1.0f, 2.0f),
        Hex(704.0f, -705.0f),
    };

    out << "Testing std::round(hex)\n";
    bool failed = false;

    for (int i = 0; i < num_tests; ++i) {
        auto actual = round(hexes[i]);
        if ((actual-expected[i]).magnitude() > EPS) {
            out << "\tstd::round(" << hexes[i] << ") gave " << actual
                << "; expected " << expected[i] << "\n";
            failed = true;
        }
    }

    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

void test_matvec_mul(ostream& out)
{
    constexpr int num_tests = 8;
    array<valarray<float>, num_tests> matrices {
        valarray<float>{0.0f, 0.0f, 0.0f, 0.0f},
        valarray<float>{1.0f, 0.0f, 0.0f, 1.0f},
        valarray<float>{2.0f, 0.0f, 0.0f, 2.0f},
        valarray<float>{-2.2f, -3.3f, 4.4f, 5.5f},
        valarray<float>{-10.4f, -13.195f, -0.89f, -14.4f},
        valarray<float>{33.0f, 33.0f, 33.0f, 33.0f},
        inv_hex_basis(23.0f, PI/6.0f), inv_hex_basis(20.0f, PI/5.0f)
    };
    array<valarray<float>, num_tests> vecs {
        valarray<float>{1.0f, 1.0f}, valarray<float>{0.0f, 0.0f},
        valarray<float>{-3.3f, 18.95f}, valarray<float>{-0.12f, -0.73f},
        valarray<float>{32.0f, 57.0f}, valarray<float>{24.0f, -17.5f},
        valarray<float>{-100.0f, 100.0f}, valarray<float>{43.0f, -31.0f}
    };
    array<valarray<float>, num_tests> expected {
        valarray<float>{0.0f, 0.0f}, valarray<float>{0.0f, 0.0f},
        valarray<float>{-6.6f, 37.9f}, valarray<float>{2.673f, -4.543f},
        valarray<float>{-1084.92f, -849.28f}, valarray<float>{214.5f, 214.5f},
        valarray<float>{-2.89855f, 3.95949f}, valarray<float>{1.31747f, -1.67848f}
    };

    out << "Testing matvec_mul\n";
    bool failed = false;

    for (int i = 0; i < num_tests; ++i) {
        auto actual = matvec_mul(matrices[i], vecs[i]);
        valarray<float> diff(expected[i]-actual);
        auto err = inner_product(begin(diff), end(diff), begin(diff), 0.0f);

        if (err > EPS) {
            failed = true;
            out << "\tmatvec_mul(" << vecstr(matrices[i]) << ", "
                << vecstr(vecs[i]) << ") gave " << vecstr(actual)
                << "; expected " << vecstr(expected[i]) << "\n";
        }
    }

    if (!failed) {
        out << "\tTest Passed!\n";
    }
}

void test_hex_map(ostream& out)
{
    constexpr int num_tests = 5;
    array<int, num_tests> radii { 0, 1, 3, 4, 73 };
    array<pair<Hex, bool>, num_tests> coords {
        make_pair(Hex(0.0f, 0.0f), true), make_pair(Hex(1.0f, 1.0f), false),
        make_pair(Hex(0.0f, -3.0f), true), make_pair(Hex(0.0f, -3.0f), true),
        make_pair(Hex(-100.0f, -200.0f), false)
    };

    out << "Testing grid.hex_map\n";
    bool failed = false;

    for (int i = 0; i < num_tests; ++i) {
        auto set = Grid::hex_map(radii[i]);
        auto [h, expected] = coords[i];
        bool actual = set.find(h) != set.end();

        if (expected != actual) {
            failed = true;
            out << "\t" << h << " is " << (actual? "" : "not") << " in the map;"
                << " expected it " << (expected? "" : "not") << " to be.";
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
        test_Grid, test_hex_to_cartesian, test_vertices, test_cartesian_to_hex, 
        test_hex_round, test_matvec_mul, test_hex_map
    };

    for (auto test : tests) {
        test(cout);
    }
}
