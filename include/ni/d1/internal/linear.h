#pragma once

#include <span>

#include "ni/util/concept.h"
#include "alg_helper.h"

namespace ni::d1::detail {
    template<concepts::num Num>
    constexpr auto linear_formula(
        Num x0, Num y0,
        Num x1, Num y1,
        Num xq
    ) noexcept -> Num {
        return y0 + (y1 - y0) * (xq - x0) / (x1 - x0);
    }

    template<concepts::num Num>
    constexpr auto linear_eval(
        std::span<const Num> x,
        std::span<const Num> y,
        Num xq
    ) noexcept -> Num {
        const auto idx = lower_idx(x, xq);
        const auto x0 = x[idx - 1], x1 = x[idx];
        const auto y0 = y[idx - 1], y1 = y[idx];
        return linear_formula(x0, y0, x1, y1, xq);
    }
}
