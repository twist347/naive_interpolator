#pragma once

#include <span>
#include <cmath>

#include "ni_alg_helper.h"
#include "../util/ni_concept.h"

namespace ni::d1::detail {
    // nearest left value
    template<number Number>
    constexpr auto nearest_eval(
        std::span<const Number> x,
        std::span<const Number> y,
        Number xq
    ) noexcept -> Number {
        const auto idx = lower_idx(x, xq);
        const auto prev_distance = std::abs(xq - x[idx - 1]);
        const auto next_distance = std::abs(x[idx] - xq);
        return prev_distance < next_distance ? y[idx - 1] : y[idx];
    }
}
