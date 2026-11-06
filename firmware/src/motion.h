// motion.h — Motion module
// Translates high-level actions into left/right wheel speeds.
#pragma once

enum class Action { Stop, Forward, Backward, SpinLeft, SpinRight };

namespace Motion {
    void init();
    void execute(Action a);
    // Future: void driveStraight(int speed); void turnDegrees(int deg);
}
