#pragma once

#include <cstddef>
#include <cstdint>

// Optional host pad provider. When set, PSPadBackend::readState calls it first; returning true means `data` (32-byte
// DualShock2 reply: [0]=0x01? [1]=0x73 analog, [2..3]=buttons active-low, [4..7]=RX,RY,LX,LY, [8..19]=pressures)
// was filled and the built-in raylib mapping is skipped.
using PS2PadProvider = bool (*)(int port, int slot, uint8_t *data, size_t size);
using PS2PadRumbleProvider = void (*)(int port, int slot, uint8_t smallMotor, uint8_t largeMotor);

void ps2SetPadProvider(PS2PadProvider provider);
void ps2SetPadRumbleProvider(PS2PadRumbleProvider provider);
void ps2SubmitPadRumble(int port, int slot, uint8_t smallMotor, uint8_t largeMotor);
