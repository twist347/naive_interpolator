#pragma once

// TODO: implement

#include <variant>

#include "owner.h"

namespace ni::d1 {
    template<concepts::num Num, typename Allocator = std::allocator<Num> >
    class any_owner {
    public:
        constexpr any_owner(any_owner &&) noexcept = default;

        constexpr auto operator=(any_owner &&) noexcept -> any_owner & = default;

        // move-only
        constexpr any_owner(const any_owner &) = delete;

        constexpr auto operator=(const any_owner &) -> any_owner & = delete;

        constexpr auto x() const noexcept -> std::span<const Num> {
            return std::visit([](const auto &i) noexcept { return i.x(); }, m_interp);
        }

        constexpr auto y() const noexcept -> std::span<const Num> {
            return std::visit([](const auto &i) noexcept { return i.y(); }, m_interp);
        }

    private:
        using var = std::variant<
            owner<kind::prev, Num, Allocator>,
            owner<kind::next, Num, Allocator>,
            owner<kind::nearest, Num, Allocator>,
            owner<kind::linear, Num, Allocator>,
            owner<kind::quadratic, Num, Allocator>,
            owner<kind::cubic, Num, Allocator>
        >;

        var m_interp;

        // TODO: hides move ctor
        template<typename Interp>
        explicit constexpr any_owner(Interp &&interp) : m_interp(std::forward<Interp>(interp)) {
        }
    };
}
