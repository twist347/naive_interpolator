#pragma once

#include <cstdint>

#include "../util/ni_concept.h"

namespace ni::d1 {
    enum class boundary : std::uint8_t {
        ub = 0,
        clamp,
        extrapolate,
        fill // left, right
    };

    enum class x_order : std::uint8_t {
        check_strict,
        assume_strict,
        sort_pairs // owner only
    };

    template<number Number>
    struct interp_options {
        boundary bnd = boundary::ub;
        x_order order = x_order::check_strict;

        // uses only when boundary bnd == boundary::fill
        Number left_fill = Number{};
        Number right_fill = Number{};
    };
}