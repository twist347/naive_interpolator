#pragma once

#include <expected>
#include <ranges>
#include <vector>
#include <span>
#include <new>
#include <concepts>

#include "ni_interp_base.h"

namespace ni::d1 {
    template<method M, number Number, typename Allocator = std::allocator<Number> >
    class owner : public detail::base<owner<M, Number, Allocator>, M, Number> {
    public:
        using value_type = Number;
        using error = interp_error;
        using result = std::expected<owner, error>;
        using data_holder = std::vector<Number, Allocator>;

        // forbid copies

        constexpr owner(const owner &) = delete;

        constexpr auto operator=(const owner &) -> owner & = delete;

        // only moves

        constexpr owner(owner &&) noexcept = default;

        constexpr auto operator=(owner &&) noexcept -> owner & = default;

        constexpr auto x() const noexcept -> std::span<const Number> { return m_x; }
        constexpr auto y() const noexcept -> std::span<const Number> { return m_y; }

        // constexpr auto as_view() const noexcept -> interp_view<M, Value> { return interp_view<M, Value>{x(), y()}; }

    private:
        // friend external factories
        template<method MM, typename RX, typename RY>
            requires contig_sized_and_number<RX, RY>
        friend constexpr auto make_owner(
            RX &&x,
            RY &&y
        ) noexcept -> owner<MM, std::ranges::range_value_t<RX> >::result;

        template<method MM, typename RX, typename RY>
            requires contig_sized_and_number<RX, RY>
        friend constexpr auto make_owner_try(
            RX &&x,
            RY &&y
        ) noexcept -> owner<MM, std::ranges::range_value_t<RX> >;

        // private factories

        [[nodiscard]] constexpr static auto make(
            std::span<const Number> x,
            std::span<const Number> y
        ) noexcept -> result {
            if (const auto err = detail::validate_inputs<M>(x, y)) {
                return std::unexpected{*err};
            }
            try {
                return owner{x, y};
            } catch (const std::bad_alloc &) {
                return std::unexpected{error::bad_alloc};
            } catch (...) {
                return std::unexpected{error::unexpected_exception};
            }
        }

        // try wrapper

        [[nodiscard]] constexpr static auto make_try(
            std::span<const Number> x,
            std::span<const Number> y
        ) noexcept -> owner {
            auto res = make(x, y);
            if (!res.has_value()) {
                ni::log_fatal("failed to make interp: {}", detail::err_to_str(res.error()));
            }
            return std::move(res.value());
        }

        constexpr owner(std::span<const Number> x, std::span<const Number> y)
            : m_x{x.cbegin(), x.cend()},
              m_y{y.cbegin(), y.cend()} {
        }

        data_holder m_x{};
        data_holder m_y{};
    };

    // external factories

    // TODO: what about allocator?

    template<method M, typename RX, typename RY>
        requires contig_sized_and_number<RX, RY>
    [[nodiscard]] constexpr auto make_owner(
        RX &&x,
        RY &&y
    ) noexcept -> owner<M, std::ranges::range_value_t<RX> >::result {
        using V = std::ranges::range_value_t<RX>;
        auto sx = std::span<const V>{std::ranges::data(x), std::ranges::size(x)};
        auto sy = std::span<const V>{std::ranges::data(y), std::ranges::size(y)};
        return owner<M, V>::make(sx, sy);
    }

    template<method M, typename RX, typename RY>
        requires contig_sized_and_number<RX, RY>
    [[nodiscard]] constexpr auto make_owner_try(
        RX &&x,
        RY &&y
    ) noexcept -> owner<M, std::ranges::range_value_t<RX> > {
        using V = std::ranges::range_value_t<RX>;
        auto sx = std::span<const V>{std::ranges::data(x), std::ranges::size(x)};
        auto sy = std::span<const V>{std::ranges::data(y), std::ranges::size(y)};
        return owner<M, V>::make_try(sx, sy);
    }
}
