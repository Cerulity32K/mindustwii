#include <array>

#include "renderer.hpp"
#include "vertex.hpp"

local gfx::vertex VERTICES[] ATTRIBUTE_ALIGN(32) = {
    {{-1, -1, 0}, {255, 0, 0, 255}},
    {{1, -1, 0}, {0, 255, 0, 255}},
    {{-1, 1, 0}, {0, 0, 255, 255}},
    {{1, 1, 0}, {0, 0, 0, 255}},
};
local u16 QUAD_INDICES[]{0, 1, 2, 3, 2, 1};

local constexpr fn make_quad(gfx::color color) -> std::array<gfx::vertex, 4> {
    return {
        gfx::vertex{{-1, -1, 0}, color, {0, 0}},
        gfx::vertex{{1, -1, 0}, color, {1, 0}},
        gfx::vertex{{-1, 1, 0}, color, {0, 1}},
        gfx::vertex{{1, 1, 0}, color, {1, 1}},
    };
}

namespace gfx {
fn draw_square(f32 angle, math::vec2f center, math::vec2f size, color color)
    -> void {
    Mtx transform;
    Mtx scale;
    Mtx rotate;
    Mtx translate;

    guMtxScale(scale, size.x, size.y, 1);
    guMtxRotRad(rotate, 'z', angle);
    guMtxTrans(translate, center.x, center.y, 0);
    guMtxConcat(rotate, scale, transform);
    guMtxConcat(translate, transform, transform);
    graphics::set_view(transform);

    let vertices = make_quad(color);
    graphics::draw_indexed(
        VERTICES, QUAD_INDICES, 6, graphics::draw_flags::none
    );
}
} // namespace gfx
