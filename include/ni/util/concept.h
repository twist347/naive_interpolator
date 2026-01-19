#pragma once

#include <concepts>
#include <type_traits>
#include <ranges>

#include "range.h"

namespace ni::concepts::detail {
    template<typename RX, typename RY>
    concept same_range_value_type = std::same_as<ranges::value_type<RX>, ranges::value_type<RY> >;

    template<typename R>
    concept contig_sized_range = std::ranges::contiguous_range<std::remove_cvref_t<R> >
                                 && std::ranges::sized_range<std::remove_cvref_t<R> >;
}

namespace ni::concepts {
    template<typename T>
    concept num = std::floating_point<std::remove_cvref_t<T> >;

    template<typename R>
    concept owner_range = detail::contig_sized_range<R> && num<ranges::value_type<R> >;

    template<typename RX, typename RY>
    concept owner_xy = owner_range<RX> && owner_range<RY> && detail::same_range_value_type<RX, RY>;

    template<typename R>
    concept view_range = owner_range<R> && std::is_lvalue_reference_v<R &&>;

    template<typename RX, typename RY>
    concept view_xy = view_range<RX> && view_range<RY> && detail::same_range_value_type<RX, RY>;
}
