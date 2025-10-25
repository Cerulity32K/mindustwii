#include "../math/math.hpp"

namespace game {

class unit {
public:
    math::vec2f position;
    math::vec2f velocity;
    f32 angle;
    f32 target_angle;

    struct settings {
        /// Velocity scales by this factor once per second if left alone.
        /// For example, 0.5 will halve the unit's speed every second
        /// (continuously).
        f32 drag;
        /// The movement speed of the unit. A stronger drag will slow the unit
        /// down.
        f32 move_speed;
        /// How fast the unit can rotate to its target angle.
        f32 rotate_speed;
    } settings;

    fn update(math::vec2f acceleration_direction, f32 dt) -> void;
};
} // namespace game
