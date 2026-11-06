// main.ino — wiring of modules. No control logic lives here.
#include "config.h"
#include "motor_driver.h"
#include "motion.h"
#include "sensors.h"
#include "comm.h"
#include "supervisor.h"

void setup() {
    // MotorDriver::init();
    // Sensors::init();
    // Comm::init();
    // Supervisor::init();
}

void loop() {
    // Fixed-period loop (LOOP_PERIOD_MS):
    //   Sensors::update();
    //   Command cmd = Comm::pollCommand();
    //   Action a = Supervisor::step(cmd, Sensors::state());
    //   Motion::execute(a);
    //   Comm::sendTelemetry(Supervisor::mode(), Sensors::state());
}
