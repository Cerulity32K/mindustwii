#include "audio.hpp"

#include <cmath>

#include <gcutil.h>
#include <ogc/lwp.h>
#include <ogc/lwp_watchdog.h>

namespace audio {
constexpr usize FILLER_STACK_SIZE = 8192;

u8 audio_buffer[BUFFER_SIZE] ATTRIBUTE_ALIGN(32);
mut sample_index = u64{0};

lwp_t audio_thread;
lwpq_t audio_queue;

u64 last_audio_time;
u64 last_sample_index;

// for monotonicity
mut last_returned_time = 0.0;
fn get_time() -> f64 {
    let current_time = gettime();
    let time_delta = ticks_to_nanosecs(current_time - last_audio_time);
    let result = sample_index / 48000.0 + time_delta / 1'000'000'000.0 - 0.1;
    if (result > last_returned_time) {
        last_returned_time = result;
    }
    return result;
}

u64 get_sample_index() {
    return sample_index;
}

u8 player_stack[FILLER_STACK_SIZE] ATTRIBUTE_ALIGN(8);
void audio_callback(AESNDPB* pb, u32 state) {
    switch (state) {
    case VOICE_STATE_STOPPED:
    case VOICE_STATE_RUNNING:
        break;
    case VOICE_STATE_STREAM:
        AESND_SetVoiceBuffer(pb, audio_buffer, BUFFER_SIZE);
        LWP_ThreadSignal(audio_queue);
    }
}

void* audio_filler_wrapper(void (*audio_filler)(u8*)) {
    while (true) {
        LWP_ThreadSleep(audio_queue);
        last_audio_time = gettime();
        last_sample_index = sample_index;
        audio_filler(audio_buffer);
        sample_index += BUFFER_SIZE;
    }
}

void initialise(void (*audio_filler)(u8* data)) {
    AESND_Init();

    AESNDPB* channel = AESND_AllocateVoice(audio_callback);
    AESND_SetVoiceFormat(channel, VOICE_MONO8_UNSIGNED);
    AESND_SetVoiceFrequency(channel, 48000);
    AESND_SetVoiceVolume(channel, 255, 255);
    AESND_SetVoiceStream(channel, true);
    AESND_SetVoiceStop(channel, false);

    LWP_InitQueue(&audio_queue);
    LWP_CreateThread(
        &audio_thread,
        (void* (*)(void*))audio_filler_wrapper,
        (void*)audio_filler,
        player_stack,
        FILLER_STACK_SIZE,
        80
    );

    sample_index = 0;
    last_returned_time = 0;
    last_audio_time = gettime();
    last_sample_index = sample_index;
}
} // namespace audio
