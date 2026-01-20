#pragma once

#include <cstdio>
#include <cstdlib>
#include <utility>
#include <cassert>
#include <format>

namespace ni::log {
    enum class level {
        info = 0,
        warn,
        error,
        fatal,
        count
    };

    namespace detail {
        constexpr auto level_name(level lvl) noexcept -> const char * {
            assert(lvl < level::count);

            switch (lvl) {
                case level::info: return "INFO";
                case level::warn: return "WARN";
                case level::error: return "ERROR";
                case level::fatal: return "FATAL";
                default: return "UNKNOWN";
            }
        }

        constexpr auto stream_for(level lvl) noexcept -> FILE * {
            assert(lvl < level::count);

            switch (lvl) {
                case level::info:
                case level::warn:
                    return stdout;
                case level::error:
                case level::fatal:
                default:
                    return stderr;
            }
        }
    }

    template<typename... Args>
    auto log(level lvl, std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        assert(lvl < level::count);

        FILE *out = detail::stream_for(lvl);
        try {
            const auto prefix = std::format("[{}]: ", detail::level_name(lvl));
            const auto message = std::vformat(fmt.get(), std::make_format_args(args...));

            std::fputs(prefix.c_str(), out);
            std::fputs(message.c_str(), out);
            std::fputc('\n', out);
        } catch (...) {
            std::fputs("[LOG]: formatting failed\n", stderr);
        }

        if (lvl == level::fatal) {
            std::abort();
        }
    }

    template<typename... Args>
    auto log_info(std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        log(level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto log_warn(std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        log(level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto log_error(std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        log(level::error, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto log_fatal(std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        log(level::fatal, fmt, std::forward<Args>(args)...);
    }
}
