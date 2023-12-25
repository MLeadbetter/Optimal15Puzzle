#include <cstdint>
#include <immintrin.h>

#include "grid_converter.h"

uint64_t gridToTileLocations(uint64_t grid) {
    // tileLocations and grid are permutation inverses of each other (using pshufb on them will generate numbers 0 to 15 in order).
    // Unfortunately to my knowledge this is the fastest known way to generate the inverse permutation
    uint64_t tileLocations = 0;
    for(uint64_t i = 0; i < 16; i++) {
        uint64_t tile = (grid >> i*4) & 0xf;
        tileLocations ^= i << tile*4;
    }
    return tileLocations;
}
