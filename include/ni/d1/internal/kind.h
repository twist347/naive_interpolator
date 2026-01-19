#pragma once

#include <cstdint>

namespace ni::d1 {
    enum class kind : std::uint8_t {
        prev,
        next,
        nearest,
        linear,
        quadratic,
        cubic,
    };
}