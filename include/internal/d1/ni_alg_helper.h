#pragma once

#include <cstddef>
#include <span>
#include <algorithm>

#include "../util/ni_concept.h"

namespace ni::d1::detail {
    template<number Number>
    constexpr auto upper_idx(std::span<const Number> x, Number xq) noexcept -> std::size_t {
        return static_cast<std::size_t>(std::distance(x.cbegin(), std::upper_bound(x.cbegin(), x.cend(), xq)));
    }

    template<number Number>
    constexpr auto lower_idx(std::span<const Number> x, Number xq) noexcept -> std::size_t {
        return static_cast<std::size_t>(std::distance(x.cbegin(), std::lower_bound(x.cbegin(), x.cend(), xq)));
    }
}
