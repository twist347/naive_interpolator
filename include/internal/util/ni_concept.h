#pragma once

#include <concepts>
#include <type_traits>
#include <ranges>

namespace ni::detail {
    template<typename RX, typename RY>
    concept same_value_type = std::same_as<
        std::ranges::range_value_t<std::remove_cvref_t<RX> >, std::ranges::range_value_t<std::remove_cvref_t<RY> >
    >;

    template<typename R>
    concept contig_sized_range = std::ranges::contiguous_range<std::remove_cvref_t<R> >
                                 && std::ranges::sized_range<std::remove_cvref_t<R> >;

    template<typename RX, typename RY>
    concept contig_sized_same = contig_sized_range<RX> &&
                                contig_sized_range<RY> &&
                                same_value_type<RX, RY>;
}

namespace ni {
    template<typename T>
    concept number = std::floating_point<std::remove_cvref_t<T> >;

    template<typename RX, typename RY>
    concept contig_sized_and_number = detail::contig_sized_same<RX, RY>
                                      && number<std::ranges::range_value_t<std::remove_cvref_t<RX> > >;
}
