#pragma once

#include <concepts>

namespace tess {
template<typename T>
concept numeric = std::integral<T> or std::floating_point<T>;

template<typename Point>
concept cartesian = requires(const Point p) {
    Point{};
    Point{p.x, p.y};
    numeric<std::remove_cvref_t<decltype(p.x)>>;
    std::same_as<std::remove_cvref_t<decltype(p.x)>,
                 std::remove_cvref_t<decltype(p.y)>>;
};

template<typename Point>
concept axial = requires(const Point p) {
    Point{};
    Point{p.q, p.r};
    numeric<std::remove_cvref_t<decltype(p.q)>>;
    std::same_as<std::remove_cvref_t<decltype(p.q)>,
                 std::remove_cvref_t<decltype(p.r)>>;
};

template<typename Point>
struct scalar_field;

template<cartesian Point>
struct scalar_field<Point> {
    using type = std::remove_cvref_t<decltype(std::declval<Point>().x)>;
};

template<axial Point>
struct scalar_field<Point> {
    using type = std::remove_cvref_t<decltype(std::declval<Point>().q)>;
};

template<typename Point>
using scalar_field_t = typename scalar_field<Point>::type;
}
