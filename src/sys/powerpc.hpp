#include "../rusty.hpp"

namespace ppc {
inline fn dcbf(void* address) { asm volatile("dcbf 0, %0" : : "r"(address)); }
} // namespace ppc
