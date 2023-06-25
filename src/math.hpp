#pragma once
#include "fields.hpp"
#include <concepts>
#include <iterator>
#include <cmath>

#include <span>
#include <array>

#include <ranges>
#include <algorithm>
#include <numeric>

namespace tess {

enum class hex_top { flat, pointed };

template<std::floating_point Field, std::output_iterator<Field> FieldOutput>
constexpr FieldOutput hex_to_pixel(FieldOutput into_vector,
                                   hex_top top = hex_top::pointed)
{
    namespace ranges = std::ranges;
    constexpr Field sqrt3 = 1.73205080757;
    constexpr Field two = 2;
    constexpr Field three = 3;

    if (top == hex_top::pointed) {
        return ranges::copy(std::array<Field, 4>{ sqrt3, sqrt3/2, 0, 3/two },
                            into_vector).out;
    }
    else {
        return ranges::copy(std::array<Field, 4>{ 3/two, 0, sqrt3/2, sqrt3 },
                            into_vector).out;
    }
}

template<std::floating_point Field, std::output_iterator<Field> FieldOutput>
constexpr FieldOutput pixel_to_hex(FieldOutput into_vector,
                                   hex_top top = hex_top::pointed)
{
    namespace ranges = std::ranges;
    constexpr Field sqrt3 = 1.73205080757;
    constexpr Field two = 2;
    constexpr Field three = 3;

    if (top == hex_top::pointed) {
        return ranges::copy(
            std::array<Field, 4>{ sqrt3/3, -1/three, 0, 2/three },
            into_vector
        ).out;
    }
    else {
        return ranges::copy(
            std::array<Field, 4>{ 2/three, 0, -1/three, sqrt3/3 },
            into_vector
        ).out;
    }
}

template<numeric_range Vector>
requires std::ranges::forward_range<Vector>
     and std::ranges::output_range<Vector, vector_field_t<Vector>>

constexpr auto scale_by(Vector& vec,
                        std::type_identity_t<vector_field_t<Vector>> c)
{
    auto scale_elem = [&](auto elem) { return c * elem; };
    return std::ranges::transform(vec, std::ranges::begin(vec), scale_elem);
}

template<numeric_range Lhs, numeric_range Rhs>
requires std::ranges::forward_range<Lhs> and std::ranges::input_range<Rhs>
     and std::ranges::output_range<Lhs, vector_field_t<Lhs>>
     and std::convertible_to<vector_field_t<Rhs>, vector_field_t<Lhs>>

constexpr auto add_to(Lhs& to, Rhs && amt)
{
    return std::ranges::transform(to, amt, std::ranges::begin(to),
                                  std::plus{});
}

template<numeric_range Lhs, numeric_range Rhs>
requires std::ranges::forward_range<Lhs> and std::ranges::input_range<Rhs>
     and std::ranges::output_range<Lhs, vector_field_t<Lhs>>
     and std::convertible_to<vector_field_t<Rhs>, vector_field_t<Lhs>>
constexpr auto
subtract_from(Lhs& from, Rhs && amt)
{
    return std::ranges::transform(from, amt, std::ranges::begin(from),
                                  std::minus{});
}

template<numeric_range Lhs, numeric_range Rhs>
constexpr auto
subtract(Lhs && from, Rhs && amt)
{
    return std::views::zip_transform(std::minus{}, from, amt);
}

template<numeric_range Vector>
requires std::ranges::forward_range<Vector>
     and std::ranges::output_range<Vector, vector_field_t<Vector>>

constexpr auto abs(Vector& vec)
{
    return std::ranges::transform(vec, std::ranges::begin(vec),
                                  [](auto elem) { return std::abs(elem); });
}

template<numeric_range Vector,
         std::output_iterator<vector_field_t<Vector>> Output>
requires std::ranges::input_range<Vector>
constexpr auto abs(Vector && vec, Output into_abs)
{
    return std::ranges::transform(vec, into_abs,
                                  [](auto elem) { return std::abs(elem); });
}

template<numeric_range Vector>
requires std::ranges::forward_range<Vector>
     and std::ranges::output_range<Vector, vector_field_t<Vector>>

constexpr auto round(Vector& vec)
{
    return std::ranges::transform(vec, std::ranges::begin(vec),
                                  [](auto elem) { return std::round(elem); });
}

template<numeric_range Vector,
         std::output_iterator<vector_field_t<Vector>> Output>
requires std::ranges::input_range<Vector>
constexpr auto round(Vector& vec, Output into_rounded)
{
    return std::ranges::transform(vec, into_rounded,
                                  [](auto elem) { return std::round(elem); });
}

template<numeric_range Lhs, numeric_range Rhs>
requires std::ranges::input_range<Lhs> and std::ranges::input_range<Rhs>
     and std::convertible_to<vector_field_t<Lhs>, vector_field_t<Rhs>>

constexpr vector_field_t<Rhs> 
dot(Lhs && a, Rhs && b)
{
    return std::inner_product(std::ranges::begin(a), std::ranges::end(a),
                              std::ranges::begin(b), 0);
}

template<numeric_range Matrix, numeric_range Vector, numeric_range To>
requires std::ranges::forward_range<Matrix>
     and std::ranges::sized_range<Matrix>

     and std::ranges::forward_range<Vector>
     and std::ranges::sized_range<Vector>

     and std::ranges::output_range<To, vector_field_t<To>>
     and std::convertible_to<vector_field_t<Matrix>, vector_field_t<Vector>>
     and std::convertible_to<vector_field_t<Vector>, vector_field_t<To>>

constexpr auto
matmul(const Matrix& mat, const Vector& vec, To& product)
{
    constexpr auto M = std::ranges::size(mat);
    constexpr auto N = std::ranges::size(vec);

    auto row_mul = [=](auto i) {
        return dot(std::span(mat.begin()+(i*N), N), vec);
    };
    return std::ranges::transform(std::views::iota(0uz, M/N),
                                  std::ranges::begin(product), row_mul);
}
}
