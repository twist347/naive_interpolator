#pragma once

#include <cstdint>

namespace ni::d1 {
    enum class method : std::uint8_t {
        prev,
        next,
        nearest,
        linear,
        quadratic,
        cubic,
    };
}