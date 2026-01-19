#pragma once

#include <span>
#include <cmath>

#include "ni/util/concept.h"
#include "alg_helper.h"

namespace ni::d1::detail {
    // nearest left value
    template<concepts::num Num>
    constexpr auto nearest_eval(
        std::span<const Num> x,
        std::span<const Num> y,
        Num xq
    ) noexcept -> Num {
        const auto idx = lower_idx(x, xq);
        const auto prev_distance = std::abs(xq - x[idx - 1]);
        const auto next_distance = std::abs(x[idx] - xq);
        return prev_distance < next_distance ? y[idx - 1] : y[idx];
    }
}
