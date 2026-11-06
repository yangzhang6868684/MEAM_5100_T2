# Firmware

ESP32 firmware skeleton. See `docs/software_plan.md` for the architecture.

- `src/` — one header per module. Implementations go in matching `.cpp` files.
- `data/` — files served by the ESP32 web server (upload with LittleFS).

Build: Arduino IDE or PlatformIO with the ESP32 core. Board and library
versions to be pinned once chosen.
