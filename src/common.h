#pragma once

typedef void (*set_resolution_t)(int w, int h);

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

constexpr int tilemapScrensWidth = 2;
constexpr int tilemapScreensHeight = 2;
constexpr int totalTilemapSizeTiles = tilemapScrensWidth * tilemapScreensHeight * maxScreenWidthTiles * maxScreenHeightTiles;
constexpr int numTilemaps = 4;

constexpr int numTilePages = 256;
constexpr int tilesPerPage = 256;
constexpr int pageSizePixels = tilesPerPage * tileSizePixels;
constexpr int pageSizeBytes = pageSizePixels * sizeof(uint32_t);

constexpr int totalTilesSizePixels = tileSizePixels * tilesPerPage * numTilePages;
constexpr int totalTilesSizeBytes = totalTilesSizePixels * sizeof(uint32_t);
	