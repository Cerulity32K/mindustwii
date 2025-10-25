#pragma once

#include "../sys/graphics.hpp"

#include <ogc/gu.h>

namespace gfx {
fn draw_square(f32 angle, math::vec2f center, math::vec2f size = {1, 1})
    -> void;
} // namespace gfx
