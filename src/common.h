#pragma once

typedef void (*set_resolution_t)(int w, int h);
constexpr int buttonMapping[12] = {6, 8, 5, 4, 0, 1, 2, 3, 7, 9, 10, 11};

constexpr auto fps = 30;
constexpr auto audioSampleRate = 22050;
constexpr auto samplesPerTick = audioSampleRate / fps;
constexpr auto maxPlayers = 4;
	
constexpr int tileWidth = 8;
constexpr int tileHeight = 8;
constexpr int tileSizePixels = tileWidth * tileHeight;
constexpr int tileSizeBytes = tileSizePixels * sizeof(uint32_t);

constexpr int maxScreenWidthTiles = 40;
constexpr int maxScreenHeightTiles = 40;

constexpr int maxScreenWidthPixels = maxScreenWidthTiles * tileWidth;
constexpr int maxScreenHeightPixels = maxScreenHeightTiles * tileHeight;
constexpr int maxScreenTotalPixels = maxScreenWidthPixels * maxScreenHeightPixels;

constexpr int tilemapScreensWidth = 16;
constexpr int tilemapScreensHeight = 16;
constexpr int tilemapWidthTiles = tilemapScreensWidth * maxScreenWidthTiles;
constexpr int tilemapHeightTiles = tilemapScreensHeight * maxScreenHeightTiles;
constexpr int tilemapWidthPixels = tilemapWidthTiles * tileWidth;
constexpr int tilemapHeightPixels = tilemapHeightTiles * tileHeight;
constexpr int tilemapSizeTiles =  tilemapWidthTiles * tilemapHeightTiles;
constexpr int tilemapSizePixels = tilemapWidthPixels * tilemapHeightPixels;

constexpr int numTilePages = 256;
constexpr int tilesPerPage = 256;
constexpr int pageSizePixels = tilesPerPage * tileSizePixels;
constexpr int pageSizeBytes = pageSizePixels * sizeof(uint32_t);

constexpr int totalTilesSizePixels = tileSizePixels * tilesPerPage * numTilePages;
constexpr int totalTilesSizeBytes = totalTilesSizePixels * sizeof(uint32_t);
	