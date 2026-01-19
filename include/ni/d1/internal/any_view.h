#pragma once

// TODO: implement

#include <variant>

#include "view.h"

namespace ni::d1 {
    template<concepts::num Num>
    class any_view {
    public:
        constexpr any_view(any_view &&) noexcept = default;

        constexpr auto operator=(any_view &&) noexcept -> any_view & = default;

        // move-only
        constexpr any_view(const any_view &) = delete;

        constexpr auto operator=(const any_view &) -> any_view & = delete;

        constexpr auto x() const noexcept -> std::span<const Num> {
            return std::visit([](const auto &i) noexcept { return i.x(); }, m_interp);
        }

        constexpr auto y() const noexcept -> std::span<const Num> {
            return std::visit([](const auto &i) noexcept { return i.y(); }, m_interp);
        }

    private:
        using var = std::variant<
            view<kind::prev, Num>,
            view<kind::next, Num>,
            view<kind::nearest, Num>,
            view<kind::linear, Num>,
            view<kind::quadratic, Num>,
            view<kind::cubic, Num>
        >;

        var m_interp;

        // TODO: hides move ctor
        template<typename Interp>
        explicit constexpr any_view(Interp &&interp) : m_interp(std::forward<Interp>(interp)) {
        }
    };
}
