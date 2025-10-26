#pragma once

#include <ogc/gx.h>

#include "video.hpp"
#include "../gfx/vertex.hpp"
#include "../macros.hpp"
#include "../rusty.hpp"

namespace graphics {
enum class draw_flags : u32 {
    none = 0,
    invalidate_cache = 1,
};
IMPL_ENUM_CLASS_FLAGS(draw_flags, u32);

/// Initialises the graphics subsystem with the result of video initialisation.
fn initialise(video::initialisation_data data) -> void;
/// Sets up the proper vertex attributes for the `vertex_t` type.
fn setup_vertex_attributes() -> void;

/// Draws an array of `vertex` elements as triangles. Every 3 vertices make
/// one triangle.
fn draw_array(
    const gfx::vertex* vertices, u16 count, draw_flags flags = draw_flags::none
) -> void;
/// Draws an array of `vertex` elements as triangles, given indices into the
/// vertex array. Every 3 indices make one triangle.
fn draw_indexed(
    const gfx::vertex* vertices,
    u16* indices,
    u16 index_count,
    draw_flags flags = draw_flags::none
) -> void;

/// Sets the clear color.
fn set_clear_color(GXColor color) -> void;
/// Starts a render pass.
fn start_drawing() -> void;
/// Sets the view matrix.
fn set_view(Mtx view) -> void;
/// Sets the orthographic projection matrix.
fn set_orthographic_projection(Mtx44 view) -> void;
/// Sets the perspective projection matrix.
fn set_perspective_projection(Mtx44 view) -> void;
/// Ends a render pass.
fn stop_drawing() -> void;
} // namespace graphics
