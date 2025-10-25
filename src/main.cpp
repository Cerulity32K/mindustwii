#include "sys/cache.hpp"
#include "sys/graphics.hpp"
#include "sys/input.hpp"
#include "sys/audio.hpp"
#include "sys/video.hpp"

#include "math/math.hpp"

#include "gfx/vertex.hpp"
#include "gfx/renderer.hpp"

#include "rusty.hpp"
#include "wiiuse/wiiuse.h"

#include "game/unit.hpp"
#include "game/factory.hpp"

#include <cstdlib>

#include <ogc/console.h>
#include <ogc/conf.h>
#include <ogc/gu.h>
#include <ogc/system.h>
#include <wiiuse/wpad.h>

fn audio_filler(u8* buffer) {
    for (usize i = 0; i < audio::BUFFER_SIZE; i++) {
        let t = (u64)(audio::get_sample_index() + i) / 6;
        // u64 t = (audio::get_sample_index() + i) / 6 * 8192 / 8000;

        // uint8_t neurofunk =
        //     t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + (1 & t >>
        //     14)
        //         ) >>
        //         (3 & -t >> (t & 2048 ? 2 : 10)) |
        //     t >> (t & 16384 ? t & 4096 ? -1 : 2 : 3);

        // u64 drum_parts[] = {
        //     2,
        //     2,
        //     (t & 0xfff) * (t & 0xfff) / 5,
        //     0,
        //     1,
        //     1,
        //     (t & 0xfff) * (t & 0xfff) / 5,
        //     0
        // };
        // f64 grind_drums = 3e3 / sqrt(t * drum_parts[t >> 11 & 7] % 2048);

        // uint8_t greaser_drums =
        //     (t >> 9 & (1 + t) >> 12 & 7        ? 0
        //      : (9001 / (t % 4096) - t / 9) & 8 ? 255
        //                                        : 0);
        // buffer[i] =
        //     greaser_drums * 0.4 + ((size_t)grind_drums ^ neurofunk) % 256 *
        //     0.6;

        // buffer[i] = t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) +
        //                  (1 & t >> 14)) >>
        //                 (3 & -t >> (t & 2048 ? 2 : 10)) |
        //             t >> (t & 16384 ? t & 4096 ? -1 : 3 : 2);

        // buffer[i] = (t>>9&(1+t)>>12&7?0:9001/(t%4096)-t/9&8?-1:0);

        // buffer[i] = (t >> 9 & (1 + t) >> 12 & 7      ? 0
        //              : 9001 / (t % 4096) - t / 9 & 8 ? -1
        //                                              : 0) ^
        //             (t >> 4 & t * (t >> 5 | t >> 8));
        // buffer[i] =
        //     t * t * 4 / ((5656 >> (t >> 12 & 14) & 7) + 9) * (t >> 10 & 893)
        //     &
        //         t >> 4 ^
        //     ((t >> 16 & 1 ? 0 : t >> 10 & -t >> 14 & 3) |
        //              t >> 9 & (1 + t) >> 12 & 7
        //          ? 0
        //      : 9001 / (t % 4096) - t / 9 & 8 ? -1
        //                                      : 0);
    }
}

f32 aspect_ratio;
f32 screen_width;
f32 screen_height;
fn initialise() {
    let initialisation_data = video::initialise(false);
    screen_width = (f32)initialisation_data.screen_mode->viWidth;
    screen_height = (f32)initialisation_data.screen_mode->viHeight;
    aspect_ratio =
        CONF_GetAspectRatio() == CONF_ASPECT_16_9 ? 16.0 / 9.0 : 4.0 / 3.0;
    graphics::initialise(initialisation_data);
    audio::initialise(audio_filler);
    input::initialise();
}

fn main() -> int {
    Mtx view;
    Mtx44 projection;

    initialise();

    mut camera = guVector{0.0F, 0.0F, -2.0F};
    let up = guVector{0.0F, 1.0F, 0.0F};
    mut look = guVector{0.0F, 0.0F, 0.0F};

    graphics::setup_vertex_attributes();

    let input_channel = input::channel{0};

    mut last_time = audio::get_time();

    mut player = game::unit{
        .settings =
            {
                .drag = 0.01,
                .move_speed = 50,
                .rotate_speed = 6,
            }
    };
    mut factory = game::factory{};
    factory.place(std::make_unique<game::block>(), {0, 0});
    factory.place(std::make_unique<game::block>(), {0, 1});
    factory.place(std::make_unique<game::block>(), {0, 2});
    factory.place(std::make_unique<game::big_block>(), {0, 3});
    factory.place(std::make_unique<game::big_block>(), {0, 5});

    while (1) {
        let audio_time = audio::get_time() / 8192 * 8000;
        let dt = audio_time - last_time;
        let t = (u64)(audio::get_time() * 8000);
        let c = (u8)(~(t >> 5 | t >> 8));
        let zoom = 3.0f;

        // input processing
        input::scan();
        let input_state = input_channel();

        let player_acceleration =
            input_state.nunchuck_tilt(0.1).value_or_else([&]() {
                return input_state.dpad_direction();
            });
        player.update(player_acceleration, dt);

        let cursor_world_position =
            input_state.ir_position().map([&](math::vec2f position) {
                return math::vec2f{
                    position.x * zoom * aspect_ratio + player.position.x,
                    position.y * zoom + player.position.y,
                };
            });
        cursor_world_position.then([&](math::vec2f position) {
            let block_position = game::block_position{
                (i32)floorf(position.x), (i32)floorf(position.y)
            };
            if (input_state.was_pressed(input::button::a)) {
                factory.place(std::make_unique<game::block>(), block_position);
            }
            if (input_state.was_pressed(input::button::b)) {
                factory.place(
                    std::make_unique<game::big_block>(), block_position
                );
            }
            if (input_state.was_pressed(input::button::minus)) {
                factory.destroy(block_position);
            }
        });

        // drawing and graphics
        camera.x = player.position.x;
        camera.y = player.position.y;
        look.x = camera.x;
        look.y = camera.y;

        guOrtho(
            projection,
            zoom + player.position.y,
            -zoom + player.position.y,
            -zoom * aspect_ratio + player.position.x,
            zoom * aspect_ratio + player.position.x,
            0,
            1000
        );

        graphics::set_orthographic_projection(projection);

        graphics::set_clear_color(
            {(u8)(audio_time * 256),
             (u8)(audio_time * 128),
             (u8)(audio_time * 64),
             255}
        );
        graphics::start_drawing();
        factory.draw();
        gfx::draw_square(player.angle, player.position, {0.25, 0.25});

        cursor_world_position.then([&](math::vec2f ir_position) {
            gfx::draw_square(0, ir_position, {0.1, 0.1});
        });

        graphics::stop_drawing();

        if (input_state.was_pressed(input::button::plus)) {
            exit(0);
        }

        last_time = audio_time;
    }
    return 0;
}
