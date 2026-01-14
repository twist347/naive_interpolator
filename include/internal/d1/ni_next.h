#pragma once

#include <span>

#include "ni_alg_helper.h"
#include "../util/ni_concept.h"

namespace ni::d1::detail {
    template<number Number>
    constexpr auto next_eval(
        std::span<const Number> x,
        std::span<const Number> y,
        Number xq
    ) noexcept -> Number {
        const auto idx = lower_idx(x, xq);
        return y[idx];
    }
}
