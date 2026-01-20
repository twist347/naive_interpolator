#pragma once

#include <expected>
#include <ranges>
#include <vector>
#include <span>
#include <new>
#include <concepts>

#include "base.h"

// for as_view()
#include "view.h"

namespace ni::d1 {
    template<kind K, concepts::num Num, typename Allocator = std::allocator<Num> >
    class owner : public detail::base<owner<K, Num, Allocator>, K, Num> {
    public:
        using value_type = Num;
        using error = interp_error;
        using result = std::expected<owner, error>;
        using data_holder = std::vector<Num, Allocator>;

        // forbid copies

        constexpr owner(const owner &) = delete;
        constexpr auto operator=(const owner &) -> owner & = delete;

        // only moves

        constexpr owner(owner &&) noexcept = default;
        constexpr auto operator=(owner &&) noexcept -> owner & = default;

        constexpr auto x() const noexcept -> std::span<const Num> { return m_x; }
        constexpr auto y() const noexcept -> std::span<const Num> { return m_y; }

        // NOTE: returns a non-owning view over this owner's storage.
        // call on objects created by factories only
        constexpr auto as_view() const noexcept -> view<K, Num> { return view<K, Num>{x(), y()}; }

    private:
        using base = detail::base<owner, K, Num>;

        // friend external factories
        template<kind KK, typename RX, typename RY>
            requires concepts::owner_xy<RX, RY>
        friend constexpr auto make_owner(
            RX &&x,
            RY &&y
        ) noexcept -> owner<KK, ranges::value_type<RX> >::result;

        template<kind KK, typename RX, typename RY>
            requires concepts::owner_xy<RX, RY>
        friend constexpr auto make_owner_try(
            RX &&x,
            RY &&y
        ) noexcept -> owner<KK, ranges::value_type<RX> >;

        // private factories

        [[nodiscard]] constexpr static auto make(
            std::span<const Num> x,
            std::span<const Num> y
        ) noexcept -> result {
            if (const auto err = base::validate_inputs(x, y)) {
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
            std::span<const Num> x,
            std::span<const Num> y
        ) noexcept -> owner {
            auto res = make(x, y);
            if (!res.has_value()) {
                log::log_fatal("failed to make interp: {}", base::err_to_str(res.error()));
            }
            return std::move(res.value());
        }

        constexpr owner(std::span<const Num> x, std::span<const Num> y)
            : m_x{x.cbegin(), x.cend()},
              m_y{y.cbegin(), y.cend()} {
        }

        data_holder m_x{};
        data_holder m_y{};
    };

    // external factories

    // TODO: what about allocator?

    template<kind K, typename RX, typename RY>
        requires concepts::owner_xy<RX, RY>
    [[nodiscard]] constexpr auto make_owner(
        RX &&x,
        RY &&y
    ) noexcept -> owner<K, ranges::value_type<RX> >::result {
        using N = ranges::value_type<RX>;
        auto sx = ranges::to_span(x);
        auto sy = ranges::to_span(y);
        return owner<K, N>::make(sx, sy);
    }

    template<kind K, typename RX, typename RY>
        requires concepts::owner_xy<RX, RY>
    [[nodiscard]] constexpr auto make_owner_try(
        RX &&x,
        RY &&y
    ) noexcept -> owner<K, ranges::value_type<RX> > {
        using N = ranges::value_type<RX>;
        auto sx = ranges::to_span(x);
        auto sy = ranges::to_span(y);
        return owner<K, N>::make_try(sx, sy);
    }
}
