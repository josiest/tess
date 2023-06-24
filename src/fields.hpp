#pragma once
#include <concepts>

namespace tess {
/** A numeric type that can be used as fields for points and hexes.
 *
 * \remark Although integers are not mathematically considered a field,
 *         integer types may still be used as fields for points and hexes.
 */
template<typename Field>
concept numeric_field = requires{
    requires std::floating_point<Field> or std::integral<Field>;
    requires not std::same_as<Field, bool>;
};
}
