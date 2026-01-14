#pragma once

#include <algorithm>
#include <span>
#include <execution>
#include <utility>
#include <optional>
#include <type_traits>
#include <cstddef>

#include "../util/ni_concept.h"
#include "../util/ni_log.h"

#include "ni_method.h"
#include "ni_prev.h"
#include "ni_next.h"
#include "ni_nearest.h"
#include "ni_linear.h"
#include "ni_quadratic.h"
#include "ni_cubic.h"

namespace ni::d1 {
    enum class interp_error {
        bad_alloc = 0,
        empty_input,
        too_few_points,
        x_not_strictly_increasing,
        sizes_mismatch,
        unexpected_exception,
    };
}

namespace ni::d1::detail {
    template<method M, number Number>
    constexpr auto interp_dispatch(
        std::span<const Number> x,
        std::span<const Number> y,
        Number xq
    ) noexcept -> Number {
        if constexpr (M == method::prev) {
            return prev_eval(x, y, xq);
        } else if constexpr (M == method::next) {
            return next_eval(x, y, xq);
        } else if constexpr (M == method::nearest) {
            return nearest_eval(x, y, xq);
        } else if constexpr (M == method::linear) {
            return linear_eval(x, y, xq);
        } else if constexpr (M == method::quadratic) {
            return quadratic_eval(x, y, xq);
        } else if constexpr (M == method::cubic) {
            return cubic_eval(x, y, xq);
        } else {
            static_assert(false, "Unsupported interpolation method");
            return Number{};
        }
    }

    template<method M>
    constexpr auto min_points_for() noexcept -> std::size_t {
        if constexpr (M == method::prev || M == method::next || M == method::nearest) {
            return 1;
        } else if constexpr (M == method::linear) {
            return 2;
        } else if constexpr (M == method::quadratic) {
            return 3;
        } else if constexpr (M == method::cubic) {
            return 4;
        } else {
            static_assert(false);
            return 0;
        }
    }

    template<method M, number Number>
    constexpr auto validate_inputs(
        std::span<const Number> x,
        std::span<const Number> y
    ) noexcept -> std::optional<interp_error> {
        if (x.empty() || y.empty()) {
            return interp_error::empty_input;
        }
        if (x.size() != y.size()) {
            return interp_error::sizes_mismatch;
        }
        if (x.size() < min_points_for<M>()) {
            return interp_error::too_few_points;
        }

        const auto bad = std::adjacent_find(
            x.cbegin(), x.cend(),
            [](Number a, Number b) { return !(a < b); }
        );
        if (bad != x.cend()) {
            return interp_error::x_not_strictly_increasing;
        }

        return std::nullopt;
    }

    template<number Number>
    constexpr auto ensure_batch_sizes(
        std::span<const Number> xqs,
        std::span<Number> out,
        const char *func_name
    ) noexcept -> void {
        if (out.size() < xqs.size()) {
            ni::log_fatal("{}: out.size() ({}) < xqs.size() ({})", func_name, out.size(), xqs.size());
        }
    }

    constexpr static auto err_to_str(interp_error err) noexcept -> const char * {
        switch (err) {
            case interp_error::bad_alloc:
                return "bad_alloc";
            case interp_error::empty_input:
                return "empty_input";
            case interp_error::too_few_points:
                return "too_few_points";
            case interp_error::x_not_strictly_increasing:
                return "x_not_strictly_increasing";
            case interp_error::sizes_mismatch:
                return "sizes_mismatch";
            case interp_error::unexpected_exception:
                return "unexpected_exception";
            default:
                return "unknown error";
        }
    }

    template<typename Derived, method M, number Number>
    class base {
    private:
        constexpr auto self() const noexcept -> const Derived & {
            return *static_cast<const Derived *>(this);
        }

    public:
        constexpr auto operator()(Number xq) const noexcept -> Number {
            return detail::interp_dispatch<M>(self().x(), self().y(), xq);
        }

        constexpr auto eval(Number xq) const noexcept -> Number {
            return operator()(xq);
        }

        // batch versions

        constexpr auto operator()(std::span<const Number> xqs, std::span<Number> out) const noexcept -> void {
            // TODO: think about it
            detail::ensure_batch_sizes(xqs, out, __func__);

            std::transform(
                xqs.cbegin(), xqs.cend(), out.begin(),
                [this](Number xq) noexcept -> Number {
                    return operator()(xq);
                }
            );
        }

        constexpr auto eval(std::span<const Number> xqs, std::span<Number> out) const noexcept -> void {
            operator()(xqs, out);
        }

        // batch + execution policies versions

        template<typename ExecPolicy>
            requires std::is_execution_policy_v<std::remove_cvref_t<ExecPolicy> >
        auto operator()(
            ExecPolicy &&exec_policy,
            std::span<const Number> xqs,
            std::span<Number> out
        ) const -> void {
            // TODO: think about it
            detail::ensure_batch_sizes(xqs, out, __func__);

            std::transform(
                std::forward<ExecPolicy>(exec_policy),
                xqs.cbegin(), xqs.cend(), out.begin(),
                [this](Number xq) -> Number {
                    return operator()(xq);
                }
            );
        }

        template<typename ExecPolicy>
            requires std::is_execution_policy_v<std::remove_cvref_t<ExecPolicy> >
        auto eval(
            ExecPolicy &&exec_policy, std::span<const Number> xqs, std::span<Number> out
        ) const -> void {
            operator()(std::forward<ExecPolicy>(exec_policy), xqs, out);
        }
    };
}
