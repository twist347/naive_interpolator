#pragma once

#include <span>

#include "ni_alg_helper.h"
#include "../util/ni_concept.h"

namespace ni::d1::detail {
    template<number Number>
    constexpr auto quadratic_formula(
        Number x0, Number y0,
        Number x1, Number y1,
        Number x2, Number y2,
        Number xq
    ) noexcept -> Number {
        return y0 * (xq - x1) * (xq - x2) / ((x0 - x1) * (x0 - x2)) +
               y1 * (xq - x0) * (xq - x2) / ((x1 - x0) * (x1 - x2)) +
               y2 * (xq - x0) * (xq - x1) / ((x2 - x0) * (x2 - x1));
    }

    template<number Number>
    constexpr auto quadratic_eval(
        std::span<const Number> x,
        std::span<const Number> y,
        Number xq
    ) noexcept -> Number {
        const auto n = x.size();
        const auto idx = lower_idx(x, xq);

        std::size_t i0 = 0, i1 = 1, i2 = 2;
        if (idx >= 2) {
            if (idx + 1 < n) {
                i0 = idx - 1;
                i1 = idx;
                i2 = idx + 1;
            } else {
                i0 = n - 3;
                i1 = n - 2;
                i2 = n - 1;
            }
        }
        return quadratic_formula(
            x[i0], y[i0],
            x[i1], y[i1],
            x[i2], y[i2],
            xq
        );
    }
}
