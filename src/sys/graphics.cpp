#include <ogc/gx.h>
#include <ogc/gx_struct.h>
#include <ogc/video.h>

#include "graphics.hpp"
#include "video.hpp"
#include "cache.hpp"

#include "../rusty.hpp"

namespace graphics {

GXRModeObj* screen_mode;
fn initialise(video::initialisation_data data) -> void {
    screen_mode = data.screen_mode;

    GXColor background_color = {0, 0, 0, 255};
    GX_Init(data.fifo_buffer, video::FIFO_SIZE);
    set_clear_color(background_color);

    GX_SetViewport(0, 0, screen_mode->fbWidth, screen_mode->efbHeight, 0, 1);
    GX_SetDispCopyYScale(
        (f32)screen_mode->xfbHeight / (f32)screen_mode->efbHeight
    );
    GX_SetScissor(0, 0, screen_mode->fbWidth, screen_mode->efbHeight);
    GX_SetDispCopySrc(0, 0, screen_mode->fbWidth, screen_mode->efbHeight);
    GX_SetDispCopyDst(screen_mode->fbWidth, screen_mode->xfbHeight);
    GX_SetCopyFilter(
        screen_mode->aa,
        screen_mode->sample_pattern,
        GX_TRUE,
        screen_mode->vfilter
    );
    GX_SetFieldMode(
        screen_mode->field_rendering,
        ((screen_mode->viHeight == 2 * screen_mode->xfbHeight) ? GX_ENABLE
                                                               : GX_DISABLE)
    );

    GX_SetCullMode(GX_CULL_NONE);
    GX_CopyDisp(data.framebuffer, GX_TRUE);
    GX_SetDispCopyGamma(GX_GM_1_0);
}

fn setup_vertex_attributes() -> void {
    GX_ClearVtxDesc();

    GX_SetVtxDesc(GX_VA_POS, GX_INDEX16);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

    GX_SetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

    GX_SetNumChans(1);
    GX_SetNumTexGens(0);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
}

fn draw_preamble(gfx::vertex* vertices, u16 count, draw_flags flags) -> void {
    if (!!(flags & draw_flags::invalidate_cache)) {
        cache::flush_data_range(vertices, sizeof(gfx::vertex) * count);
    }

    GX_SetArray(GX_VA_POS, &vertices[0].position, sizeof(gfx::vertex));
    GX_SetArray(GX_VA_CLR0, &vertices[0].color, sizeof(gfx::vertex));

    GX_Begin(GX_TRIANGLES, GX_VTXFMT0, count);
}

fn draw_indexed(
    gfx::vertex* vertices, u16* indices, u16 index_count, draw_flags flags
) -> void {
    draw_preamble(vertices, index_count, flags);

    for (u16 i = 0; i < index_count; i++) {
        u16 index = indices[i];
        GX_Position1x16(index);
        GX_Color1x16(index);
    }

    GX_End();
}

fn draw_array(gfx::vertex* vertices, u16 count, draw_flags flags) -> void {
    draw_preamble(vertices, count, flags);

    for (u16 i = 0; i < count; i++) {
        GX_Position1x16(i);
        GX_Color1x16(i);
    }

    GX_End();
}

fn set_clear_color(GXColor new_clear_color) -> void {
    GX_SetCopyClear(new_clear_color, 0x00ffffff);
}

fn start_drawing() -> void {
    GX_SetViewport(0, 0, screen_mode->fbWidth, screen_mode->efbHeight, 0, 1);
    GX_InvVtxCache();
    GX_InvalidateTexAll();
}

fn set_view(Mtx view) -> void {
    Mtx modelView;

    guMtxIdentity(modelView);
    guMtxConcat(view, modelView, modelView);

    GX_LoadPosMtxImm(modelView, GX_PNMTX0);
}

fn set_orthographic_projection(Mtx44 projection) -> void {
    GX_LoadProjectionMtx(projection, GX_ORTHOGRAPHIC);
}
fn set_perspective_projection(Mtx44 projection) -> void {
    GX_LoadProjectionMtx(projection, GX_PERSPECTIVE);
}

fn stop_drawing() -> void {
    GX_DrawDone();
    video::mark_ready_for_copy();

    VIDEO_WaitVSync();
}
} // namespace graphics
