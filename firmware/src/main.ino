// main.ino — wiring of modules. No control logic lives here.
#include "config.h"
#include "motor_driver.h"
#include "motion.h"
#include "sensors.h"
#include "comm.h"
#include "tophat.h"
#include "supervisor.h"

void setup() {
    // MotorDriver::init();
    // Sensors::init();
    // TopHat::init();     // shared I2C bus with Sensors
    // Comm::init();
    // Supervisor::init();
}

void loop() {
    // Fixed-period loop (LOOP_PERIOD_MS):
    //   Sensors::update();
    //   Command cmd = Comm::pollCommand();
    //   Comm passes usage counts to TopHat; counting rules TBD.
    //   TopHat::update(); // internal timers for polling and 500 ms reports
    //   Action a = Supervisor::step(cmd, Sensors::state(), TopHat::state());
    //   Stop conditions take priority over movement.
    //   Motion::execute(a);
    //   Comm::sendTelemetry(Supervisor::mode(), Sensors::state());
}
