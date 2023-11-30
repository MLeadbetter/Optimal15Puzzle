
#include <cstdint>

#include "grid_converter.h"

uint64_t gridToTileLocations(uint64_t grid) {
    uint64_t tileLocations = 0;
    for(uint64_t i = 0; i < 16; i++) {
        uint64_t tile = (grid >> i*4) & 0xf;
        tileLocations ^= i << tile*4;
    }
    return tileLocations;
}
