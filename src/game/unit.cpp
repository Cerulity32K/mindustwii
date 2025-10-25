#include "unit.hpp"

namespace game {
fn unit::update(math::vec2f acceleration_direction, f32 dt) -> void {
    velocity = velocity + acceleration_direction.clamp_to_normal() * dt *
                              settings.move_speed;
    velocity = velocity * powf(settings.drag, dt);
    position = position + velocity * dt;
    if (velocity.length() > 0.01) {
        target_angle = velocity.atan2();
    } else {
        target_angle = angle;
    }
    angle = math::turn_towards(angle, target_angle, settings.rotate_speed * dt);
}
} // namespace game
