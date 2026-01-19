#pragma once

#include <cstdint>

#include "ni/util/concept.h"

namespace ni::d1 {
    enum class boundary : std::uint8_t {
        ub = 0,
        clamp,
        extrapolate,
        fill // left, right
    };

    enum class x_order : std::uint8_t {
        check_strict = 0,
        assume_strict,
        sort_pairs // owner only
    };

    template<concepts::num Num>
    struct options {
        boundary bnd = boundary::ub;
        x_order order = x_order::check_strict;

        // uses only when boundary bnd == boundary::fill
        Num left_fill = Num{};
        Num right_fill = Num{};
    };
}