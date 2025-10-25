#pragma once

// TODO: compound assignment on vector types, also more vector types

#include "ops.hpp"
#include "../rusty.hpp"

namespace math {
template <typename T> fn ceil_div(T lhs, T rhs) -> T {
    return (lhs + rhs - 1) / rhs;
}
/// Computes a Euclidean remainder (sign of result is based on the divisor
/// instead of the dividend).
fn rem_euclid(f32 dividend, f32 divisor) -> f32;
/// Clamps a value between two bounds.
fn clamp(f32 x, f32 min, f32 max) -> f32;
/// Returns the amount `start` must turn by to reach `target`. Positive is CW,
/// negative is CCW. Unit is radians.
fn angle_difference(f32 start, f32 target) -> f32;
/// Turns an angle towards a target, with a maximum spin. Unit is radians.
fn turn_towards(f32 start, f32 target, f32 max_turn) -> f32;

typedef struct vec3d {
    f64 x;
    f64 y;
    f64 z;

    fn to_vec3f() -> vec3f_t { return {(f32)x, (f32)y, (f32)z}; }
} vec3d_t;
IMPL_XYZ_BINARY_OPERATORS(vec3d, f64)
IMPL_XYZ_UNARY_OPERATORS(vec3d)

typedef struct vec2f {
    f32 x;
    f32 y;

    fn extend(f32 with) -> vec3f_t { return {x, y, with}; }
    fn atan2() -> f32 { return atan2f(y, x); }
    fn length_squared() -> f32 { return x * x + y * y; }
    fn length() -> f32 { return sqrtf(length_squared()); }
    fn normalise() -> vec2f {
        let magnitude = length();
        return {x / magnitude, y / magnitude};
    }
    fn normalise_or_zero() -> vec2f {
        return x == 0 && y == 0 ? vec2f{0, 0} : normalise();
    }
    fn clamp_to_normal() {
        if (length_squared() > 1) {
            return normalise();
        } else {
            return *this;
        }
    }
} vec2f_t;
IMPL_XY_BINARY_OPERATORS(vec2f, f32)
IMPL_XY_UNARY_OPERATORS(vec2f)

} // namespace math
