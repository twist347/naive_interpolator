#pragma once

#include <algorithm>
#include <span>
#include <ranges>
#include <execution>
#include <utility>
#include <optional>
#include <type_traits>
#include <cstddef>

#include "ni/util/concept.h"
#include "ni/util/log.h"

#include "ni/d1/internal/kind.h"
#include "ni/d1/internal/prev.h"
#include "ni/d1/internal/next.h"
#include "ni/d1/internal/nearest.h"
#include "ni/d1/internal/linear.h"
#include "ni/d1/internal/quadratic.h"
#include "ni/d1/internal/cubic.h"

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
    template<typename ...>
    inline constexpr bool dependent_false_v = false;

    template<typename Derived, kind K, concepts::num Num>
    class base {
    private:
        constexpr auto self() const noexcept -> const Derived & {
            return *static_cast<const Derived *>(this);
        }

        static constexpr auto min_points_for() noexcept -> std::size_t {
            if constexpr (K == kind::prev || K == kind::next || K == kind::nearest) {
                return 1;
            } else if constexpr (K == kind::linear) {
                return 2;
            } else if constexpr (K == kind::quadratic) {
                return 3;
            } else if constexpr (K == kind::cubic) {
                return 4;
            } else {
                static_assert(dependent_false_v<Derived>, "Unsupported interpolation kind");
                return 0;
            }
        }

        static constexpr auto ensure_batch_sizes(
            std::span<const Num> xqs,
            std::span<Num> out,
            const char *func_name
        ) noexcept -> void {
            if (out.size() < xqs.size()) {
                log::log_fatal("{}: out.size() ({}) < xqs.size() ({})", func_name, out.size(), xqs.size());
            }
        }

    protected:
        static constexpr auto validate_inputs(
            std::span<const Num> x,
            std::span<const Num> y
        ) noexcept -> std::optional<interp_error> {
            if (x.empty() || y.empty()) {
                return interp_error::empty_input;
            }
            if (x.size() != y.size()) {
                return interp_error::sizes_mismatch;
            }
            if (x.size() < min_points_for()) {
                return interp_error::too_few_points;
            }

            const auto bad = std::adjacent_find(
                x.cbegin(), x.cend(),
                [](Num a, Num b) { return !(a < b); }
            );
            if (bad != x.cend()) {
                return interp_error::x_not_strictly_increasing;
            }

            return std::nullopt;
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

    public:
        constexpr auto operator()(Num xq) const noexcept -> Num {
            // dispatching
            if constexpr (K == kind::prev) {
                return prev_eval(self().x(), self().y(), xq);
            } else if constexpr (K == kind::next) {
                return next_eval(self().x(), self().y(), xq);
            } else if constexpr (K == kind::nearest) {
                return nearest_eval(self().x(), self().y(), xq);
            } else if constexpr (K == kind::linear) {
                return linear_eval(self().x(), self().y(), xq);
            } else if constexpr (K == kind::quadratic) {
                return quadratic_eval(self().x(), self().y(), xq);
            } else if constexpr (K == kind::cubic) {
                return cubic_eval(self().x(), self().y(), xq);
            } else {
                static_assert(false, "Unsupported interpolation kind");
                return Num{};
            }
        }

        constexpr auto eval(Num xq) const noexcept -> Num {
            return operator()(xq);
        }

        // batch versions

        constexpr auto operator()(std::span<const Num> xqs, std::span<Num> out) const noexcept -> void {
            // TODO: think about it
            ensure_batch_sizes(xqs, out, __func__);

            std::transform(
                xqs.cbegin(), xqs.cend(), out.begin(),
                [this](Num xq) noexcept -> Num {
                    return operator()(xq);
                }
            );
        }

        constexpr auto eval(std::span<const Num> xqs, std::span<Num> out) const noexcept -> void {
            operator()(xqs, out);
        }

        // batch + execution policies versions

        template<typename ExecPolicy>
            requires std::is_execution_policy_v<std::remove_cvref_t<ExecPolicy> >
        auto operator()(
            ExecPolicy &&exec_policy,
            std::span<const Num> xqs,
            std::span<Num> out
        ) const -> void {
            // TODO: think about it
            ensure_batch_sizes(xqs, out, __func__);

            std::transform(
                std::forward<ExecPolicy>(exec_policy),
                xqs.cbegin(), xqs.cend(), out.begin(),
                [this](Num xq) -> Num {
                    return operator()(xq);
                }
            );
        }

        template<typename ExecPolicy>
            requires std::is_execution_policy_v<std::remove_cvref_t<ExecPolicy> >
        auto eval(
            ExecPolicy &&exec_policy, std::span<const Num> xqs, std::span<Num> out
        ) const -> void {
            operator()(std::forward<ExecPolicy>(exec_policy), xqs, out);
        }
    };
}
