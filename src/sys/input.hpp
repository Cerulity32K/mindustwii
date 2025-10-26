#pragma once

#include <gctypes.h>
#include <ogc/lwp_watchdog.h>
#include <wiiuse/wpad.h>

#include "../math/math.hpp"
#include "../krystl.hpp"
#include "../rusty.hpp"

namespace input {

fn initialise() -> void;
fn scan() -> void;

enum class button : u32 {
    two = WPAD_BUTTON_2,
    one = WPAD_BUTTON_1,
    b = WPAD_BUTTON_B,
    a = WPAD_BUTTON_A,
    minus = WPAD_BUTTON_MINUS,
    home = WPAD_BUTTON_HOME,
    left = WPAD_BUTTON_LEFT,
    right = WPAD_BUTTON_RIGHT,
    down = WPAD_BUTTON_DOWN,
    up = WPAD_BUTTON_UP,
    plus = WPAD_BUTTON_PLUS,
};

/// The captured input from a frame.
struct state {
    u32 down;
    u32 held;
    u32 up;

    WPADData* data;

    state(int channel)
        : down{WPAD_ButtonsDown(channel)},
          held{WPAD_ButtonsHeld(channel)},
          up{WPAD_ButtonsUp(channel)},
          data{WPAD_Data(channel)} {}

    /// Whether or not the given button was just pressed.
    fn was_pressed(u32 button) const -> bool { return down & button; }
    /// Whether or not the given button is held down.
    fn is_held(u32 button) const -> bool { return held & button; }
    /// Whether or not the given button was just released.
    fn was_released(u32 button) const -> bool { return up & button; }

    /// Whether or not the given button was just pressed.
    fn was_pressed(button button) const -> bool {
        return was_pressed((u32)(button));
    }
    /// Whether or not the given button is held down.
    fn is_held(button button) const -> bool { return is_held((u32)(button)); }
    /// Whether or not the given button was just released.
    fn was_released(button button) const -> bool {
        return was_released((u32)(button));
    }

    fn ir_position() const -> kstl::option<math::vec2f> {
        let normalised_x = data->ir.sx / 320 - 1.5f;
        let normalised_y = data->ir.sy / 240 - 2;
        return data->data_present
                   ? kstl::some(math::vec2f{normalised_x, -normalised_y})
                   : kstl::none;
    }

    /// The Cartesian position of the Nunchuk joystick.
    fn nunchuck_tilt(f32 deadzone) const -> kstl::option<math::vec2f> {
        if (!data->data_present || data->exp.type != EXP_NUNCHUK) {
            return kstl::none;
        }
        let angle = data->exp.nunchuk.js.ang / 180 * M_PI;
        let magnitude =
            MAX((data->exp.nunchuk.js.mag - deadzone) / (1 - deadzone), 0);
        return kstl::some(math::vec2f{
            sinf(angle) * magnitude,
            cosf(angle) * magnitude,
        });
    }

    fn dpad_direction() const -> math::vec2f {
        mut out = math::vec2f{};
        if (is_held(button::right)) {
            out.x++;
        }
        if (is_held(button::left)) {
            out.x--;
        }
        if (is_held(button::up)) {
            out.y++;
        }
        if (is_held(button::down)) {
            out.y--;
        }
        return out.normalise_or_zero();
    }
};

/// An input channel that can receive inputs.
struct channel {
    int channel_number;

    channel(int channel_number) : channel_number(channel_number) {
        WPAD_SetVRes(channel_number, 640, 480);
    }

    /// Captures the current input state in a `state`.
    state operator()() const { return state{channel_number}; }
};
} // namespace input
