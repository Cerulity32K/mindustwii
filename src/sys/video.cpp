#pragma once

#include <cstring>
#include <malloc.h>

#include <ogc/gx.h>
#include <ogc/system.h>
#include <ogc/video.h>

#include "video.hpp"
#include "ogc/console.h"

namespace video {

void* current_framebuffer;
static vu8 ready_for_copy;

void mark_ready_for_copy() { ready_for_copy = GX_TRUE; }
static void copy_buffers(u32 count __attribute__((unused))) {
    if (ready_for_copy == GX_TRUE) {
        GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GX_SetColorUpdate(GX_TRUE);
        GX_CopyDisp(current_framebuffer, GX_TRUE);
        GX_Flush();
        ready_for_copy = GX_FALSE;
    }
}
initialisation_data initialise(bool initialise_console) {
    initialisation_data data{};

    data.screen_mode = VIDEO_GetPreferredMode(NULL);

    data.framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(data.screen_mode));

    if (initialise_console) {
        CON_Init(
            data.framebuffer,
            20,
            20,
            data.screen_mode->fbWidth - 20,
            data.screen_mode->xfbHeight - 20,
            data.screen_mode->fbWidth * VI_DISPLAY_PIX_SZ
        );
    }

    VIDEO_Init();
    VIDEO_Configure(data.screen_mode);
    VIDEO_SetNextFramebuffer(data.framebuffer);
    VIDEO_SetPostRetraceCallback(copy_buffers);
    VIDEO_SetBlack(false);
    VIDEO_Flush();

    data.fifo_buffer = MEM_K0_TO_K1(memalign(32, FIFO_SIZE));
    memset(data.fifo_buffer, 0, FIFO_SIZE);

    current_framebuffer = data.framebuffer;
    return data;
}
} // namespace video
