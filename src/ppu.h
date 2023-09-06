#include <cstdint>

#include "common.h"

namespace ppu {
	
	extern set_resolution_t resolution_cb;
	
	extern uint32_t tiles[totalTilesSizeBytes];
	extern uint16_t tilemap[totalTilemapSizeTiles];
	
	extern uint32_t tiles[totalTilesSizeBytes];
	
	void init(uint32_t** buffer, set_resolution_t resovalue);
	void deinit();
	
	void loadTiles();
	
	void process();
	
	void setResolution(int w, int h);
	void drawSprite(int index, int x, int y, bool fliph, bool flipv);
	void clearScreen(int32_t color);
}