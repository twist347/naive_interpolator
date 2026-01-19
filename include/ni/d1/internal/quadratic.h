#pragma once

#include <span>

#include "ni/util/concept.h"
#include "alg_helper.h"

namespace ni::d1::detail {
    template<concepts::num Num>
    constexpr auto quadratic_formula(
        Num x0, Num y0,
        Num x1, Num y1,
        Num x2, Num y2,
        Num xq
    ) noexcept -> Num {
        return y0 * (xq - x1) * (xq - x2) / ((x0 - x1) * (x0 - x2)) +
               y1 * (xq - x0) * (xq - x2) / ((x1 - x0) * (x1 - x2)) +
               y2 * (xq - x0) * (xq - x1) / ((x2 - x0) * (x2 - x1));
    }

    template<concepts::num Num>
    constexpr auto quadratic_eval(
        std::span<const Num> x,
        std::span<const Num> y,
        Num xq
    ) noexcept -> Num {
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
