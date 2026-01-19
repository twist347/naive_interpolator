#pragma once

#include <span>

#include "ni/util/concept.h"
#include "alg_helper.h"

namespace ni::d1::detail {
    template<concepts::num Num>
    constexpr auto next_eval(
        std::span<const Num> x,
        std::span<const Num> y,
        Num xq
    ) noexcept -> Num {
        const auto idx = lower_idx(x, xq);
        return y[idx];
    }
}
