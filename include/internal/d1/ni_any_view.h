#pragma once

// TODO: implement

#include <variant>

#include "ni_interp_view.h"

namespace ni::d1 {
    template<number Number>
    class any_view {
    public:
        constexpr any_view(any_view &&) noexcept = default;

        constexpr auto operator=(any_view &&) noexcept -> any_view & = default;

        // move-only
        constexpr any_view(const any_view &) = delete;

        constexpr auto operator=(const any_view &) -> any_view & = delete;

        constexpr auto x() const noexcept -> std::span<const Number> {
            return std::visit([](const auto &i) noexcept { return i.x(); }, m_interp);
        }

        constexpr auto y() const noexcept -> std::span<const Number> {
            return std::visit([](const auto &i) noexcept { return i.y(); }, m_interp);
        }

    private:
        using var = std::variant<
            view<method::prev, Number>,
            view<method::next, Number>,
            view<method::nearest, Number>,
            view<method::linear, Number>,
            view<method::quadratic, Number>,
            view<method::cubic, Number>
        >;

        var m_interp;

        template<typename Interp>
        explicit constexpr any_view(Interp &&interp) : m_interp(std::forward<Interp>(interp)) {
        }
    };
}
