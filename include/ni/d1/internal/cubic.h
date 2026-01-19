#pragma once

#include <span>

#include "ni/util/concept.h"
#include "alg_helper.h"

namespace ni::d1::detail {
    template<concepts::num Num>
    constexpr auto cubic_formula(
        Num x0, Num y0,
        Num x1, Num y1,
        Num x2, Num y2,
        Num x3, Num y3,
        Num xq
    ) noexcept -> Num {
        return y0 * ((xq - x1) * (xq - x2) * (xq - x3)) / ((x0 - x1) * (x0 - x2) * (x0 - x3)) +
               y1 * ((xq - x0) * (xq - x2) * (xq - x3)) / ((x1 - x0) * (x1 - x2) * (x1 - x3)) +
               y2 * ((xq - x0) * (xq - x1) * (xq - x3)) / ((x2 - x0) * (x2 - x1) * (x2 - x3)) +
               y3 * ((xq - x0) * (xq - x1) * (xq - x2)) / ((x3 - x0) * (x3 - x1) * (x3 - x2));
    }

    template<concepts::num Num>
    constexpr auto cubic_eval(
        std::span<const Num> x,
        std::span<const Num> y,
        Num xq
    ) noexcept -> Num {
        const auto n = x.size();
        const auto idx = lower_idx(x, xq);

        std::size_t i0 = 0, i1 = 1, i2 = 2, i3 = 3;
        if (idx >= 2) {
            if (idx + 1 < n) {
                i0 = idx - 2;
                i1 = idx - 1;
                i2 = idx;
                i3 = idx + 1;
            } else {
                i0 = n - 4;
                i1 = n - 3;
                i2 = n - 2;
                i3 = n - 1;
            }
        }

        return cubic_formula(
            x[i0], y[i0],
            x[i1], y[i1],
            x[i2], y[i2],
            x[i3], y[i3],
            xq
        );
    }
}
