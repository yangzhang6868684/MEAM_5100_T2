# Firmware

ESP32 firmware skeleton. Module implementations are pending.

- `src/` — one header per module. Implementations will go in matching `.cpp` files.
- `data/` — files served by the ESP32 web server (upload with LittleFS).

See the [software plan](../docs/software_plan.md) and [TopHat interface](../docs/tophat_module.md).

Build: Arduino IDE or PlatformIO with the ESP32 core. Board and library
versions to be pinned once chosen.
