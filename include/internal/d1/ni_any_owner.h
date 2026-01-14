#pragma once

// TODO: implement

#include <variant>

#include "ni_interp_owner.h"

namespace ni::d1 {
    template<number Number, typename Allocator = std::allocator<Number> >
    class any_owner {
    public:
        constexpr any_owner(any_owner &&) noexcept = default;

        constexpr auto operator=(any_owner &&) noexcept -> any_owner & = default;

        // move-only
        constexpr any_owner(const any_owner &) = delete;

        constexpr auto operator=(const any_owner &) -> any_owner & = delete;

        constexpr auto x() const noexcept -> std::span<const Number> {
            return std::visit([](const auto &i) noexcept { return i.x(); }, m_interp);
        }

        constexpr auto y() const noexcept -> std::span<const Number> {
            return std::visit([](const auto &i) noexcept { return i.y(); }, m_interp);
        }

    private:
        using var = std::variant<
            owner<method::prev, Number, Allocator>,
            owner<method::next, Number, Allocator>,
            owner<method::nearest, Number, Allocator>,
            owner<method::linear, Number, Allocator>,
            owner<method::quadratic, Number, Allocator>,
            owner<method::cubic, Number, Allocator>
        >;

        var m_interp;

        template<typename Interp>
        explicit constexpr any_owner(Interp &&interp) : m_interp(std::forward<Interp>(interp)) {
        }
    };
}
