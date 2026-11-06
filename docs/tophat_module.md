# TopHat

Interface only. Implementation pending.

## Data flow

Comm passes Wi-Fi usage counts to TopHat. TopHat handles I2C reporting and
SP reads. Supervisor uses the returned status to allow movement or stop the
motors through MotorDriver.

```text
Comm -> TopHat -> I2C device       Wi-Fi usage
I2C device -> TopHat -> Supervisor    SP / status
```

## Requirements

From the design review, slide 10:

- Report Wi-Fi usage over I2C at 2 Hz (500 ms).
- Stop the robot when SP is zero, in both manual and autonomous modes.

## Interface

See `firmware/src/tophat.h`.

| Function | Purpose |
|---|---|
| `init()` | Initialize state and set up the shared I2C connection |
| `recordWifiUsage(units)` | Add usage reported by Comm; counting rules TBD |
| `update()` | Poll SP and send the periodic usage report |
| `state()` | Return SP, validity, last update time, and report status |

Keep SP and validity separate: a failed read is not a valid zero-SP reading.
The internal SP type does not specify the I2C packet format.

## Stop handling

Supervisor owns stop and resume decisions. Planned behavior:

- Boot in STOPPED; require valid positive SP and a new movement command.
- Stop on zero SP or missing/stale status.
- After recovery, discard old commands and wait for a new one.
- Keep operator stop and phone heartbeat checks active.

Timeouts and report-failure handling are still TBD. TopHat does not write motor pins.

## Next steps

1. Confirm this year's address, packet format, SP range, and Wi-Fi counting rules.
2. Assign SDA/SCL, bus speed, and shared-bus setup with the ToF sensor.
3. Implement SP polling and 2 Hz reporting; define timeout and retry behavior.
4. Test zero-SP stops and recovery with Supervisor and MotorDriver.

Also confirm whether zero SP must disable non-drive actuators.
