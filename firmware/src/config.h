// config.h — pin map, constants, timeouts. Single source of truth for hardware wiring.
#pragma once

// --- Motor driver (TB6612FNG) --- TODO: assign pins
// #define PIN_PWMA   ...
// #define PIN_AIN1   ...
// #define PIN_AIN2   ...
// #define PIN_PWMB   ...
// #define PIN_BIN1   ...
// #define PIN_BIN2   ...
// #define PIN_STBY   ...

// --- Sensors --- TODO: assign pins once sensor BOM is final
// #define PIN_IR_RX  ...

// --- Laboratory Wi-Fi (Station / WIFI_STA) ---
// Supply network credentials locally; this template contains no lab password.
// #define LAB_WIFI_SSID      "..."
// #define LAB_WIFI_PASSWORD  "..."
// Lecture 17 assignment for Yang Zhang's board; confirm the final robot address.
// #define WIFI_STATIC_IP     "192.168.1.169"
// #define WIFI_GATEWAY       "..."  // confirm on the laboratory network
// #define WIFI_SUBNET        "..."  // confirm on the laboratory network

// --- Timing ---
// #define LOOP_PERIOD_MS      20
// #define HEARTBEAT_TIMEOUT_MS 500
