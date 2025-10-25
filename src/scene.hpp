#pragma once

#include <ogc/gu.h>

namespace scene {
/// Abstract. Contains overridable functionality for updating, drawing, and playing audio.
class scene {
    virtual guVector camera_position() = 0;
    virtual guVector camera_target() = 0;

    virtual void update() = 0;
    virtual void draw() = 0;
};
} // namespace scene
