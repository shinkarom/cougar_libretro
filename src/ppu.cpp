#include "ppu.h"

#include <cstring>

namespace ppu {
	
	uint32_t tiles[totalTilesSizeBytes];
	tilemap_t tilemaps[numTilemaps];
	
	void init() {
		memset(tiles, totalTilesSizeBytes, 0);
		for(int i = 0; i < numTilemaps; i++) {
			memset(tilemaps[i],totalTilemapSizeTiles,0);
		}
	}
	
	void deinit() {
		
	}
	
}