# Software Plan — MEAM 5100 Team 2

Status: interfaces only; implementation pending. See [TopHat](tophat_module.md).

Team: Alexander Peppas, Sreya Chamakura, Linqi Zhe

---

## 1. System Overview

The robot is a two-wheel differential drive platform controlled by an ESP32.
The ESP32 connects to the laboratory Wi-Fi router in Station (STA) mode.
A phone or laptop on the same local network drives the robot through a web
page hosted on the ESP32. The ESP32 can also act autonomously using onboard
sensors.

### 1.1 Layers

```
+---------------------------------------------------------------+
|  Human layer      Phone browser  (web UI, no control logic)   |
+---------------------------------------------------------------+
|  Control layer    ESP32 firmware                              |
|                   Comm -> Supervisor -> Motion -> MotorDriver |
|                   Sensors ---------^                          |
|                   TopHat ----------^                          |
+---------------------------------------------------------------+
|  Hardware layer   TB6612FNG driver, 2x DC motors (6V 100RPM), |
|                   IR receiver, other sensors (TBD)            |
+---------------------------------------------------------------+
```

### 1.2 Data flow

- Manual: phone button -> Comm -> Supervisor -> Motion -> MotorDriver -> motors
- Autonomous: Sensors -> Supervisor -> Motion -> MotorDriver -> motors
- Competition status: TopHat -> Supervisor -> permitted action or motor stop
- Usage reporting: Comm -> TopHat -> I2C -> competition device (2 Hz)
- Telemetry: Sensors / Supervisor state -> Comm -> phone (status display)

---

## 2. Firmware Modules

Supervisor selects actions and handles stops. Comm passes usage counts to
TopHat. Only MotorDriver writes motor pins.

| Module | Responsibility | Depends on | Owner |
|---|---|---|---|
| MotorDriver | Drive TB6612FNG: PWM + direction pins, standby, emergency stop. Only module that touches motor pins. | HAL | TBD |
| Motion | Translate high-level actions (forward, backward, spin left, spin right, stop) into left/right wheel speeds. Future: straight-line correction, timed turns. | MotorDriver | TBD |
| Sensors | Read and preprocess each sensor into a physical quantity or boolean (e.g. IR beacon detected, beacon direction, distance). Interrupt-driven where timing matters (IR pulse frequency). | HAL | TBD |
| Comm | Connect to lab Wi-Fi in STA mode, serve the web page, receive commands, send telemetry. | — | TBD |
| TopHat | Report Wi-Fi usage over I2C at 2 Hz, read SP, and expose status/validity to Supervisor. | Shared I2C bus, verified course protocol | TBD |
| Supervisor | State machine: decides which source drives the motors (manual / autonomous / stopped). Enforces safety rules. | Comm, Sensors, TopHat, Motion | TBD |

### 2.1 Main loop (conceptual)

```
setup():
    MotorDriver.init()
    Sensors.init()
    TopHat.init()        // shared I2C bus with Sensors
    Comm.init()          // lab Wi-Fi (STA) + web server
    Supervisor.init()

loop():  // fixed period, e.g. 20 ms
    Sensors.update()
    cmd = Comm.pollCommand()
    // Comm passes usage counts to TopHat; counting rules TBD.
    TopHat.update()      // internal timers for polling and 500 ms reports
    action = Supervisor.step(cmd, Sensors.state(), TopHat.state())
    // Stop conditions take priority over movement.
    Motion.execute(action)
    Comm.sendTelemetry(Supervisor.mode(), Sensors.state())
```

---

## 3. Operating Modes and Safety Rules

| Mode | Who drives motors | Entered by |
|---|---|---|
| MANUAL | Web page buttons | Fresh direction command when safety conditions allow |
| AUTO | Supervisor using sensor data | "Auto" button on web page |
| STOPPED | Nobody (motors off) | Boot, stop button, comm timeout, zero SP, fault |

Rules:

1. In MANUAL, ordinary navigation sensor readings do not override the driver.
   TopHat and all stop conditions still override manual and autonomous motion.
2. In AUTO, a direction command returns to MANUAL only if safety conditions allow.
3. In any mode, if no message is received from the phone for N ms
   (e.g. 500 ms heartbeat timeout), the robot enters STOPPED.
4. MotorDriver exposes an emergency stop that bypasses Motion.
5. Zero TopHat SP stops the robot. Report Wi-Fi usage over I2C every 500 ms;
   the packet format and counting rules are TBD.
6. Planned: stay STOPPED at boot or while TopHat status is invalid/stale.
   On recovery, discard queued motion and require a new command. Timeout and
   report-failure handling are TBD.

---

## 4. Communication Design

- Primary network mode: Station (`WIFI_STA`). The ESP32 joins the laboratory
  router; the phone or laptop joins the same local network and accesses the
  ESP32's web server by its IP address.
- Addressing: use the course-assigned static IP for the laboratory network.
  Lecture 17 assigns `192.168.1.169` to Yang Zhang's development board; confirm
  the address for the final shared robot, gateway, and subnet before integration.
  Use the laboratory router specified by the course rather than assuming the
  general campus network is suitable.
- The ESP32 serves the web page over HTTP. The page is stored on the ESP32
  (PROGMEM or LittleFS); an external web server or Internet connection is not
  required, but the laboratory router must allow local devices to communicate.
- Control/status transport: WebSocket is preferred for press/release,
  heartbeat, and telemetry. HTTP command endpoints are acceptable for the
  first milestone. This choice is independent of STA versus AP network mode.
- Connection handling (planned): connection/reconnection must not block the
  control loop. Network loss or a heartbeat timeout enters STOPPED; after
  reconnection, require a fresh valid command before resuming motion.
- Validation: test browser-to-ESP32 reachability, command latency, and timeout
  behavior on the laboratory network with other teams present. STA mode does
  not eliminate radio interference or congestion. The 500 ms heartbeat timeout
  remains an initial value to validate under those conditions.
- Configure network credentials locally; do not include laboratory passwords
  in committed source files.
- Message set (to be finalized):
  - phone -> robot: `drive <forward|backward|left|right|stop>`, `mode <manual|auto>`, `heartbeat`
  - robot -> phone: `state <mode>`, `sensors {...}`, `battery <v>`

Course reference: Lecture 17, `510-2026C-L17-ledc-wifi`, slides 36 (STA/AP),
38 (assigned IPs), 62 (lab connection), 66 (addressing), and 71 (STA web server).

---

## 5. Sensor Plan

Sensor interfaces and pin assignments are still TBD:

| Sensor | Purpose | Interface | Processing |
|---|---|---|---|
| IR receiver (beacon) | Detect / locate IR beacon | Digital, interrupt | Measure pulse frequency, classify beacon |
| Distance (ToF / ultrasonic) | Obstacle / wall following | I2C or trigger-echo | Filter, threshold |
| Other (TBD) | | | |

Finalize this table once the hardware BOM is complete.

---

## 6. Milestones

| # | Milestone | Depends on | Target date | Owner |
|---|---|---|---|---|
| 1 | MotorDriver + Motion: 4 actions controllable over serial | Hardware assembled | TBD | TBD |
| 2 | Comm: lab Wi-Fi (STA) + ESP32-hosted web page drives robot from phone | M1 | TBD | TBD |
| 3 | One sensor stable + Supervisor runs one autonomous behavior | M1, sensor BOM | TBD | TBD |
| 4 | Multi-sensor behavior + full task logic + integration test | M2, M3 | TBD | TBD |

---

## 7. Repository Layout

```
firmware/
  src/
    main.ino          # setup/loop, wiring of modules
    motor_driver.h    # MotorDriver interface
    motion.h          # Motion interface
    sensors.h         # Sensors interface
    comm.h            # Comm interface
    tophat.h          # TopHat communication and status interface (planned)
    supervisor.h      # Supervisor state machine interface
    config.h          # pin map, constants, timeouts
  data/
    index.html        # web UI served by ESP32
docs/
  software_plan.md    # this document
  tophat_module.md    # TopHat responsibilities and integration plan
```

---

## 8. Open Questions

- Final sensor list and pin assignments.
- Current TopHat protocol, Wi-Fi usage definition, shared I2C setup, and timeouts.
- Final robot static IP, gateway/subnet, and local device reachability on the lab network.
- WebSocket vs HTTP for the first milestone.
- Whether to add wheel encoders for closed-loop speed control.
- Battery voltage monitoring on the ESP32 (ADC divider) for low-battery stop.
