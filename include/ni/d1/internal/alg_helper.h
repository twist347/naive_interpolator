#pragma once

#include <cstddef>
#include <span>
#include <algorithm>

#include "ni/util/concept.h"

// internal algs common helpers
namespace ni::d1::detail {
    template<concepts::num Num>
    constexpr auto upper_idx(std::span<const Num> x, Num xq) noexcept -> std::size_t {
        return static_cast<std::size_t>(std::distance(x.cbegin(), std::upper_bound(x.cbegin(), x.cend(), xq)));
    }

    template<concepts::num Num>
    constexpr auto lower_idx(std::span<const Num> x, Num xq) noexcept -> std::size_t {
        return static_cast<std::size_t>(std::distance(x.cbegin(), std::lower_bound(x.cbegin(), x.cend(), xq)));
    }
}
