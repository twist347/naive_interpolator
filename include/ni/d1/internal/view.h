#pragma once

#include <type_traits>
#include <expected>
#include <span>
#include <ranges>

#include "base.h"

namespace ni::d1 {
    // forward decl for friendship
    template<kind K, concepts::num Num, typename Allocator>
    class owner;

    template<kind K, concepts::num Num>
    class view : public detail::base<view<K, Num>, K, Num> {
    public:
        using value_type = Num;
        using error = interp_error;
        using result = std::expected<view, error>;
        using data_holder = std::span<const Num>;

        constexpr auto x() const noexcept -> std::span<const Num> { return m_x; }
        constexpr auto y() const noexcept -> std::span<const Num> { return m_y; }

    private:
        using base = detail::base<view, K, Num>;

        template<kind KK, concepts::num NN, typename Alloc>
        friend class owner;

        // friend external factories
        template<kind KK, typename RX, typename RY>
            requires concepts::view_xy<RX, RY>
        friend constexpr auto make_view(
            RX &&x,
            RY &&y
        ) noexcept -> view<KK, ranges::value_type<RX> >::result;

        template<kind KK, typename RX, typename RY>
            requires concepts::view_xy<RX, RY>
        friend constexpr auto make_view_try(
            RX &&x,
            RY &&y
        ) noexcept -> view<KK, ranges::value_type<RX> >;

        // private factories

        [[nodiscard]] static constexpr auto make(
            std::span<const Num> x,
            std::span<const Num> y
        ) noexcept -> result {
            if (const auto err = base::validate_inputs(x, y)) {
                return std::unexpected{*err};
            }
            return view{x, y};
        }

        [[nodiscard]] static constexpr auto make_try(
            std::span<const Num> x,
            std::span<const Num> y
        ) noexcept -> view {
            auto res = make(x, y);
            if (!res) {
                log::log_fatal("failed to make interp_view: {}", base::err_to_str(res.error()));
            }
            return std::move(res.value());
        }

        constexpr view(std::span<const Num> x, std::span<const Num> y) noexcept : m_x{x}, m_y{y} {
        }

        data_holder m_x{};
        data_holder m_y{};
    };

    // TODO: wtf with all these factories?

    // make_view factories

    template<kind K, typename RX, typename RY>
        requires concepts::view_xy<RX, RY>
    [[nodiscard]] constexpr auto make_view(
        RX &&x,
        RY &&y
    ) noexcept -> view<K, ranges::value_type<RX> >::result {
        using N = ranges::value_type<RX>;
        auto sx = ranges::to_span(x);
        auto sy = ranges::to_span(y);
        return view<K, N>::make(sx, sy);
    }

    // make_view_try factories

    template<kind K, typename RX, typename RY>
        requires concepts::view_xy<RX, RY>
    [[nodiscard]] constexpr auto make_view_try(
        RX &&x,
        RY &&y
    ) noexcept -> view<K, ranges::value_type<RX> > {
        using N = ranges::value_type<RX>;
        auto sx = ranges::to_span(x);
        auto sy = ranges::to_span(y);
        return view<K, N>::make_try(sx, sy);
    }
}
