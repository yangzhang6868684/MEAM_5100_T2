// supervisor.h — Supervisor module
// State machine deciding who drives the motors: Manual / Auto / Stopped.
// Enforces safety rules (heartbeat timeout, manual overrides auto).
#pragma once

#include "comm.h"
#include "motion.h"
#include "sensors.h"

namespace Supervisor {
    void init();
    // One control step. Returns the action Motion should execute.
    Action step(const Command& cmd, const SensorState& s);
    Mode mode();
}
