#pragma once

#include <type_traits>
#include <expected>
#include <span>
#include <ranges>

#include "ni_interp_base.h"

namespace ni::d1 {
    template<method M, number Number>
    class view : public detail::base<view<M, Number>, M, Number> {
    public:
        using value_type = Number;
        using error = interp_error;
        using result = std::expected<view, error>;
        using data_holder = std::span<const Number>;

        constexpr auto x() const noexcept -> std::span<const Number> { return m_x; }
        constexpr auto y() const noexcept -> std::span<const Number> { return m_y; }

    private:
        // friend external factories
        template<method MM, typename RX, typename RY>
            requires contig_sized_and_number<RX, RY>
        friend constexpr auto make_view(
            RX &x,
            RY &y
        ) noexcept -> view<MM, std::ranges::range_value_t<RX> >::result;

        template<method MM, typename RX, typename RY>
            requires contig_sized_and_number<RX, RY>
        friend constexpr auto make_view_try(
            RX &x,
            RY &y
        ) noexcept -> view<MM, std::ranges::range_value_t<RX> >;

        // private factories

        [[nodiscard]] static constexpr auto make(
            std::span<const Number> x,
            std::span<const Number> y
        ) noexcept -> result {
            if (const auto err = detail::validate_inputs<M>(x, y)) {
                return std::unexpected{*err};
            }
            return view{x, y};
        }

        [[nodiscard]] static constexpr auto make_try(
            std::span<const Number> x,
            std::span<const Number> y
        ) noexcept -> view {
            auto res = make(x, y);
            if (!res) {
                ni::log_fatal("failed to make interp_view: {}", detail::err_to_str(res.error()));
            }
            return std::move(res).value();
        }

        constexpr view(std::span<const Number> x, std::span<const Number> y) noexcept : m_x{x}, m_y{y} {
        }

        data_holder m_x{};
        data_holder m_y{};
    };

    template<method M, typename RX, typename RY>
        requires contig_sized_and_number<RX, RY>
    [[nodiscard]] constexpr auto make_view(
        RX &x,
        RY &y
    ) noexcept -> view<M, std::ranges::range_value_t<RX> >::result {
        using V = std::ranges::range_value_t<RX>;
        auto sx = std::span<const V>{std::ranges::data(x), std::ranges::size(x)};
        auto sy = std::span<const V>{std::ranges::data(y), std::ranges::size(y)};
        return view<M, V>::make(sx, sy);
    }

    template<method M, typename RX, typename RY>
        requires contig_sized_and_number<RX, RY>
    [[nodiscard]] constexpr auto make_view_try(
        RX &x,
        RY &y
    ) noexcept -> view<M, std::ranges::range_value_t<RX> > {
        using V = std::ranges::range_value_t<RX>;
        auto sx = std::span<const V>{std::ranges::data(x), std::ranges::size(x)};
        auto sy = std::span<const V>{std::ranges::data(y), std::ranges::size(y)};
        return view<M, V>::make_try(sx, sy);
    }
}
