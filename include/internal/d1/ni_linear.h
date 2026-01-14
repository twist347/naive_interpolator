#pragma once

#include <span>
#include <cmath>

#include "ni_alg_helper.h"
#include "../util/ni_concept.h"

namespace ni::d1::detail {
    template<number Number>
    constexpr auto linear_formula(
        Number x0, Number y0,
        Number x1, Number y1,
        Number xq
    ) noexcept -> Number {
        return y0 + (y1 - y0) * (xq - x0) / (x1 - x0);
    }

    template<number Number>
    constexpr auto linear_eval(
        std::span<const Number> x,
        std::span<const Number> y,
        Number xq
    ) noexcept -> Number {
        const auto idx = lower_idx(x, xq);
        const auto x0 = x[idx - 1], x1 = x[idx];
        const auto y0 = y[idx - 1], y1 = y[idx];
        return linear_formula(x0, y0, x1, y1, xq);
    }
}
