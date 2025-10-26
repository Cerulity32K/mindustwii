#pragma once

#include "../math/math.hpp"
#include "powerpc.hpp"

namespace cache {
inline fn flush_data_range(const void* address, usize size) {
    let count = math::ceil_div<usize>(size, 32);
    for (size_t i = 0; i < count; i++) {
        ppc::dcbf((u8*)address + i * 32);
    }
}
} // namespace cache
