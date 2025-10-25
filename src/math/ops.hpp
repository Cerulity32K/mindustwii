#pragma once

#include <cmath>
#include <ogc/gu.h>
#include <ogc/system.h>
#include "wiiuse/wiiuse.h"

#define IMPL_XYZ_COMPONENTWISE_BINARY_OPERATOR(type, component_type, op)       \
    inline constexpr type operator op(type lhs, type rhs) {                    \
        return {lhs.x op rhs.x, lhs.y op rhs.y, lhs.z op rhs.z};               \
    }                                                                          \
    inline constexpr type operator op(type lhs, component_type rhs) {          \
        return {lhs.x op rhs, lhs.y op rhs, lhs.z op rhs};                     \
    }

#define IMPL_XYZ_BINARY_OPERATORS(type, component_type)                        \
    IMPL_XYZ_COMPONENTWISE_BINARY_OPERATOR(type, component_type, +)            \
    IMPL_XYZ_COMPONENTWISE_BINARY_OPERATOR(type, component_type, -)            \
    IMPL_XYZ_COMPONENTWISE_BINARY_OPERATOR(type, component_type, *)            \
    IMPL_XYZ_COMPONENTWISE_BINARY_OPERATOR(type, component_type, /)            \
    inline constexpr type operator%(type lhs, type rhs) {                      \
        return {                                                               \
            fmodf(lhs.x, rhs.x), fmodf(lhs.y, rhs.y), fmodf(lhs.z, rhs.z)      \
        };                                                                     \
    }                                                                          \
    inline constexpr type operator%(type lhs, component_type rhs) {            \
        return {fmodf(lhs.x, rhs), fmodf(lhs.y, rhs), fmodf(lhs.z, rhs)};      \
    }

#define IMPL_XYZ_COMPONENTWISE_UNARY_OPERATOR(type, op)                        \
    inline constexpr type operator op(type value) {                            \
        return {op value.x, op value.y, op value.z};                           \
    }

#define IMPL_XYZ_UNARY_OPERATORS(type)                                         \
    IMPL_XYZ_COMPONENTWISE_UNARY_OPERATOR(type, +)                             \
    IMPL_XYZ_COMPONENTWISE_UNARY_OPERATOR(type, -)

#define IMPL_XY_COMPONENTWISE_BINARY_OPERATOR(type, component_type, op)        \
    inline constexpr type operator op(type lhs, type rhs) {                    \
        return {lhs.x op rhs.x, lhs.y op rhs.y};                               \
    }                                                                          \
    inline constexpr type operator op(type lhs, component_type rhs) {          \
        return {lhs.x op rhs, lhs.y op rhs};                                   \
    }
#define IMPL_XY_BINARY_OPERATORS(type, component_type)                         \
    IMPL_XY_COMPONENTWISE_BINARY_OPERATOR(type, component_type, +)             \
    IMPL_XY_COMPONENTWISE_BINARY_OPERATOR(type, component_type, -)             \
    IMPL_XY_COMPONENTWISE_BINARY_OPERATOR(type, component_type, *)             \
    IMPL_XY_COMPONENTWISE_BINARY_OPERATOR(type, component_type, /)             \
    inline constexpr type operator%(type lhs, type rhs) {                      \
        return {fmodf(lhs.x, rhs.x), fmodf(lhs.y, rhs.y)};                     \
    }                                                                          \
    inline constexpr type operator%(type lhs, component_type rhs) {            \
        return {fmodf(lhs.x, rhs), fmodf(lhs.y, rhs)};                         \
    }

#define IMPL_XY_COMPONENTWISE_UNARY_OPERATOR(type, op)                         \
    inline constexpr type operator op(type value) {                            \
        return {op value.x, op value.y};                                       \
    }

#define IMPL_XY_UNARY_OPERATORS(type)                                          \
    IMPL_XY_COMPONENTWISE_UNARY_OPERATOR(type, +)                              \
    IMPL_XY_COMPONENTWISE_UNARY_OPERATOR(type, -)

IMPL_XYZ_BINARY_OPERATORS(guVector, f32)
IMPL_XYZ_BINARY_OPERATORS(vec3f_t, f32)
IMPL_XYZ_UNARY_OPERATORS(guVector)
IMPL_XYZ_UNARY_OPERATORS(vec3f_t)
