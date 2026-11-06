// motor_driver.h — MotorDriver module
// Only module that touches motor pins. Drives TB6612FNG (2 channels).
#pragma once

namespace MotorDriver {
    void init();
    // left/right: signed speed, -100..100 (% duty). Sign = direction.
    void setWheels(int left, int right);
    void stop();
    void emergencyStop();   // sets STBY low; bypasses Motion
}
