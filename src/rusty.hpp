#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

#define let const auto
#define mut auto
#define moved auto&&
#define fn auto

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t usize;
typedef uintptr_t uptr;
typedef uintmax_t umax;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef ssize_t isize;
typedef intptr_t iptr;
typedef intmax_t imax;

typedef float f32;
typedef double f64;
