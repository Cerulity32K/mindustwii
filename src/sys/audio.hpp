#pragma once

#include <aesndlib.h>
#include <cstddef>
#include <gctypes.h>

#include "../rusty.hpp"

namespace audio {
constexpr size_t BUFFER_SIZE = 5760;

/// Gets the time in reference to the current audio position (sample index).
///
/// NOT monotonic, though measure are planned to be added.
fn get_time() -> f64;
/// Retrieves the current sample index. Incremented by `BUFFER_SIZE` when the audio filler is called.
/// 
/// Use `get_time()` for timing, as it does extra math to extrapolate time using data from the last call to the audio callback.
fn get_sample_index() -> u64;
/// Initialises the audio subsystem with a callback.
fn initialise(void (*audio_filler)(u8* data)) -> void;
} // namespace audio
