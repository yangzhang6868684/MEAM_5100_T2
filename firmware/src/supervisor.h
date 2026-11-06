// supervisor.h — Supervisor module
// State machine deciding who drives the motors: Manual / Auto / Stopped.
// Enforces heartbeat timeout, TopHat stop conditions, and manual/auto selection.
#pragma once

#include "comm.h"
#include "motion.h"
#include "sensors.h"
#include "tophat.h"

namespace Supervisor {
    void init();
    // Return Stop for zero SP or invalid/stale TopHat status in either mode.
    Action step(const Command& cmd, const SensorState& s, const TopHatState& topHat);
    Mode mode();
}
