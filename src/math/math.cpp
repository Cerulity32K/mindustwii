#include "math.hpp"

namespace math {
fn rem_euclid(f32 x, f32 y) -> f32 {
    return x < 0 ? fmodf(x, y) + x : fmodf(x, y);
}
fn clamp(f32 x, f32 min, f32 max) -> f32 {
    if (x < min) {
        return min;
    }
    if (x > max) {
        return max;
    }
    return x;
}
fn angle_difference(f32 start, f32 target) -> f32 {
    let cw_difference = rem_euclid(target - start, M_PI * 2);
    return cw_difference > M_PI ? cw_difference - M_PI * 2 : cw_difference;
}
fn turn_towards(f32 start, f32 target, f32 max_turn) -> f32 {
    // TODO: figure out why i cant put `start +` inside of rem_euclid without it
    // freaking out
    return start +
           rem_euclid(
               clamp(angle_difference(start, target), -max_turn, max_turn),
               M_PI * 2
           );
}
} // namespace math
