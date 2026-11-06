// TopHat I2C interface. Implementation pending.
#pragma once
#include <stdint.h>

struct TopHatState {
    uint32_t sp = 0;              // Decoded value; packet format/range TBD.
    bool valid = false;          // False before a valid read or after a timeout.
    uint32_t lastUpdateMs = 0;    // Last successful SP read.
    bool reportAttempted = false;
    bool lastReportOk = false;   // Valid only after a report attempt.
};

namespace TopHat {
    void init();
    void recordWifiUsage(uint32_t units); // Counting rules TBD.
    void update();                       // Poll SP; report usage every 500 ms.
    const TopHatState& state();
}
