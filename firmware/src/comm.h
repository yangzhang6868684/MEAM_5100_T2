// comm.h — Comm module
// Lab Wi-Fi in Station (STA) mode, web server, command intake, telemetry out.
#pragma once

struct SensorState;

enum class Mode { Manual, Auto, Stopped };

struct Command {
    bool valid;
    // TODO: fields for drive direction, mode request, heartbeat
};

namespace Comm {
    void init();                        // start lab Wi-Fi (STA) + server, serve data/index.html
    Command pollCommand();              // non-blocking; valid=false if nothing new
    unsigned long lastMessageMs();      // for heartbeat timeout
    void sendTelemetry(Mode mode, const SensorState& s);
}
