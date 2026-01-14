#pragma once

#include <span>

#include "ni_alg_helper.h"
#include "../util/ni_concept.h"

namespace ni::d1::detail {
    template<number Number>
    constexpr auto prev_eval(
        std::span<const Number> x,
        std::span<const Number> y,
        Number xq
    ) noexcept -> Number {
        const auto idx = upper_idx(x, xq);
        return y[idx - 1];
    }
}
