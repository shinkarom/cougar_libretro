#include <cstdint>

#include "common.h"

namespace ppu {
	
	extern uint32_t tiles[totalTilesSizeBytes];
	using tilemap_t = uint16_t[totalTilemapSizeTiles];
	extern tilemap_t tilemaps[numTilemaps];
	
	extern uint32_t tiles[totalTilesSizeBytes];
	
	void init(uint32_t** buffer);
	void deinit();
	
	void loadTiles();
	
	void process();
	
}