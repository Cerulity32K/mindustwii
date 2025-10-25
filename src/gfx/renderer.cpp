#include "renderer.hpp"

static gfx::vertex VERTICES[] ATTRIBUTE_ALIGN(32) = {
    {vec3f_t{-1, -1, 0}, {255, 0, 0, 255}},
    {vec3f_t{1, -1, 0}, {0, 255, 0, 255}},
    {vec3f_t{-1, 1, 0}, {0, 0, 255, 255}},
    {vec3f_t{1, 1, 0}, {0, 0, 0, 255}},
};
static u16 INDICES[]{0, 1, 2, 3, 2, 1};

namespace gfx {
fn draw_square(f32 angle, math::vec2f center, math::vec2f size) -> void {
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
    graphics::draw_indexed(VERTICES, INDICES, 6, graphics::draw_flags::none);
}
} // namespace gfx
