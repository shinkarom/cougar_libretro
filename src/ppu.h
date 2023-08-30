#include <cstdint>

namespace ppu {
	
	constexpr int tileWidth = 8;
	constexpr int tileHeight = 8;
	constexpr int tileSizePixels = tileWidth * tileHeight;
	constexpr int tileSizeBytes = tileSizePixels * sizeof(uint32_t);
	
	constexpr int maxScreenWidthTiles = 40;
	constexpr int maxScreenHeightTiles = 40;
	
	constexpr int maxScreenWidthPixels = maxScreenWidthTiles * tileWidth;
	constexpr int maxScreenHeightPixels = maxScreenHeightTiles * tileHeight;
	
	constexpr int tilemapScrensWidth = 2;
	constexpr int tilemapScreensHeight = 2;
	constexpr int totalTilemapSizeTiles = tilemapScrensWidth * tilemapScreensHeight * maxScreenWidthTiles * maxScreenHeightTiles;
	constexpr int numTilemaps = 4;
	
	constexpr int tilePages = 256;
	constexpr int tilesPerPage = 256;
	constexpr int totalTilesSizePixels = tileSizePixels * tilesPerPage * tilePages;
	constexpr int totalTilesSizeBytes = totalTilesSizePixels * sizeof(uint32_t);
	
	extern uint32_t tiles[totalTilesSizeBytes];
	using tilemap_t = uint16_t[totalTilemapSizeTiles];
	extern tilemap_t tilemaps[numTilemaps];
	
	void init();
	void deinit();
	
}