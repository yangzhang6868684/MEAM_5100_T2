// sensors.h — Sensors module
// Reads and preprocesses each sensor. Interrupt-driven where timing matters.
#pragma once

struct SensorState {
    bool beaconDetected;
    int  beaconDirection;   // TODO: define units (e.g. -1 left, 0 center, +1 right)
    int  distanceMm;        // TODO: once distance sensor is chosen
    // add fields as sensor BOM is finalized
};

namespace Sensors {
    void init();
    void update();                  // called once per loop
    const SensorState& state();
}
