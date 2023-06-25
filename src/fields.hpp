#pragma once
#include <concepts>
#include <ranges>
#include <utility>

namespace tess {
/** A numeric type that can be used as fields for points and hexes.
 *
 * \remark Although integers are not mathematically considered a field,
 *         integer types may still be used as fields for points and hexes.
 */
template<typename Field>
concept numeric = std::floating_point<Field> or std::integral<Field>;

template<typename Vector>
concept numeric_range = std::ranges::range<Vector>
                    and numeric<std::ranges::range_value_t<Vector>>;

template<numeric_range Vector>
using vector_field_t = std::ranges::range_value_t<Vector>;
}
