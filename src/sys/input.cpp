#include "input.hpp"

namespace input {
fn initialise() -> void {
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
}
fn scan() -> void { WPAD_ScanPads(); }
} // namespace input
