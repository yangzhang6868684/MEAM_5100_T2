# Software Plan — MEAM 5100 Team 2

Status: skeleton (design review draft). Fill in details as modules are implemented.

Team: Alexander Peppas, Sreya Chamakura, Linqi Zhe

---

## 1. System Overview

The robot is a two-wheel differential drive platform controlled by an ESP32.
A phone connects to the ESP32's WiFi access point and drives the robot from a
web page. The ESP32 can also act autonomously using onboard sensors.

### 1.1 Layers

```
+---------------------------------------------------------------+
|  Human layer      Phone browser  (web UI, no control logic)   |
+---------------------------------------------------------------+
|  Control layer    ESP32 firmware                              |
|                   Comm -> Supervisor -> Motion -> MotorDriver |
|                   Sensors ---------^                          |
+---------------------------------------------------------------+
|  Hardware layer   TB6612FNG driver, 2x DC motors (6V 100RPM), |
|                   IR receiver, other sensors (TBD)            |
+---------------------------------------------------------------+
```

### 1.2 Data flow

- Manual: phone button -> Comm -> Supervisor -> Motion -> MotorDriver -> motors
- Autonomous: Sensors -> Supervisor -> Motion -> MotorDriver -> motors
- Telemetry: Sensors / Supervisor state -> Comm -> phone (status display)

---

## 2. Firmware Modules

Each module has one owner, one header, and a clear interface. Modules only
talk to the module directly below them; the Supervisor is the only module
that talks to everything.

| Module | Responsibility | Depends on | Owner |
|---|---|---|---|
| MotorDriver | Drive TB6612FNG: PWM + direction pins, standby, emergency stop. Only module that touches motor pins. | HAL | TBD |
| Motion | Translate high-level actions (forward, backward, spin left, spin right, stop) into left/right wheel speeds. Future: straight-line correction, timed turns. | MotorDriver | TBD |
| Sensors | Read and preprocess each sensor into a physical quantity or boolean (e.g. IR beacon detected, beacon direction, distance). Interrupt-driven where timing matters (IR pulse frequency). | HAL | TBD |
| Comm | Start WiFi AP, serve the web page, receive commands, send telemetry. | — | TBD |
| Supervisor | State machine: decides which source drives the motors (manual / autonomous / stopped). Enforces safety rules. | Comm, Sensors, Motion | TBD |

### 2.1 Main loop (conceptual)

```
setup():
    MotorDriver.init()
    Sensors.init()
    Comm.init()          // AP + web server
    Supervisor.init()

loop():  // fixed period, e.g. 20 ms
    Sensors.update()
    cmd = Comm.pollCommand()
    action = Supervisor.step(cmd, Sensors.state())
    Motion.execute(action)
    Comm.sendTelemetry(Supervisor.state(), Sensors.state())
```

---

## 3. Operating Modes and Safety Rules

| Mode | Who drives motors | Entered by |
|---|---|---|
| MANUAL | Web page buttons | Default at boot; any direction button |
| AUTO | Supervisor using sensor data | "Auto" button on web page |
| STOPPED | Nobody (motors off) | Stop button, comm timeout, fault |

Rules:

1. In MANUAL, sensor readings are displayed but never override the driver.
2. In AUTO, pressing any direction button immediately returns to MANUAL.
3. In any mode, if no message is received from the phone for N ms
   (e.g. 500 ms heartbeat timeout), the robot enters STOPPED.
4. MotorDriver exposes an emergency stop that bypasses Motion.

---

## 4. Communication Design

- ESP32 runs in AP mode; phone joins its network and opens the page.
- Page is stored on the ESP32 (PROGMEM or LittleFS) — no internet needed.
- Transport: WebSocket preferred over plain HTTP GET for low-latency
  press/release and heartbeat. HTTP is acceptable as a first milestone.
- Message set (to be finalized):
  - phone -> robot: `drive <forward|backward|left|right|stop>`, `mode <manual|auto>`, `heartbeat`
  - robot -> phone: `state <mode>`, `sensors {...}`, `battery <v>`

---

## 5. Sensor Plan

Sensor BOM is not finalized yet (design review slide 4 is empty). Candidate list:

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
| 2 | Comm: AP + web page drives robot from phone | M1 | TBD | TBD |
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
    supervisor.h      # Supervisor state machine interface
    config.h          # pin map, constants, timeouts
  data/
    index.html        # web UI served by ESP32
docs/
  software_plan.md    # this document
```

---

## 8. Open Questions

- Final sensor list and pin assignments.
- WebSocket vs HTTP for the first milestone.
- Whether to add wheel encoders for closed-loop speed control.
- Battery voltage monitoring on the ESP32 (ADC divider) for low-battery stop.
