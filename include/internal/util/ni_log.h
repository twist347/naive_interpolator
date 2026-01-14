#pragma once

#include <cstdio>
#include <cstdlib>
#include <utility>
#include <cassert>
#include <format>

namespace ni {
    enum class log_level {
        info = 0,
        warn,
        error,
        fatal,
        count
    };

    namespace detail {
        constexpr auto level_name(log_level level) noexcept -> const char * {
            assert(level < log_level::count);

            switch (level) {
                case log_level::info: return "INFO";
                case log_level::warn: return "WARN";
                case log_level::error: return "ERROR";
                case log_level::fatal: return "FATAL";
                default: return "UNKNOWN";
            }
        }

        constexpr auto stream_for(log_level level) noexcept -> FILE * {
            assert(level < log_level::count);

            switch (level) {
                case log_level::info:
                case log_level::warn:
                    return stdout;
                case log_level::error:
                case log_level::fatal:
                default:
                    return stderr;
            }
        }
    }

    template<typename... Args>
    auto log(log_level level, std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        assert(level < log_level::count);

        FILE *out = detail::stream_for(level);
        try {
            const auto prefix = std::format("[{}]: ", detail::level_name(level));
            const auto message = std::vformat(fmt.get(), std::make_format_args(args...));

            std::fputs(prefix.c_str(), out);
            std::fputs(message.c_str(), out);
            std::fputc('\n', out);
        } catch (...) {
            std::fputs("[LOG]: formatting failed", stderr);
        }

        if (level == log_level::fatal) {
            std::abort();
        }
    }

    template<typename... Args>
    auto log_info(std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        log(log_level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto log_warn(std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        log(log_level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto log_error(std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        log(log_level::error, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto log_fatal(std::format_string<Args...> fmt, Args &&... args) noexcept -> void {
        log(log_level::fatal, fmt, std::forward<Args>(args)...);
    }
}
