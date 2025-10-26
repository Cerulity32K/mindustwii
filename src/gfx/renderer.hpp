#pragma once

#include <ogc/gu.h>

#include "../sys/graphics.hpp"

namespace gfx {
fn draw_square(
    f32 angle,
    math::vec2f center,
    math::vec2f size,
    color color
) -> void;
} // namespace gfx
