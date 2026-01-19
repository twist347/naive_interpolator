#pragma once

#include <ranges>
#include <span>
#include <type_traits>

namespace ni::ranges {
    template<typename R>
    using value_type = std::ranges::range_value_t<std::remove_cvref_t<R> >;

    template<typename R>
    constexpr auto to_span(R &&r) noexcept -> std::span<const value_type<R>> {
        return {std::ranges::data(r), std::ranges::size(r)};
    }
}
