#pragma once

#include "../math/math.hpp"

#include <ogc/lwp_queue.h>
#include <gcutil.h>
#include <wiiuse/wiiuse.h>

namespace gfx {
    /// A color type.
    typedef struct color {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } ATTRIBUTE_PACKED color_t;
    
    /// The main vertex type.
    struct vertex {
        vec3f_t position;
        color_t color;
        math::vec2f uv;
    } ATTRIBUTE_PACKED;
}
