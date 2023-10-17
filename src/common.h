#pragma once

typedef void (*set_resolution_t)(int w, int h);
constexpr int buttonMapping[12] = {6, 8, 5, 4, 0, 1, 2, 3, 7, 9, 10, 11};

constexpr auto fps = 60;
constexpr auto audioSampleRate = 22050;
constexpr auto samplesPerTick = audioSampleRate / fps;
constexpr auto maxPlayers = 4;
constexpr auto maxAudioTracks = 0xFFFF + 1;
	
constexpr int tileWidth = 8;
constexpr int tileHeight = 8;
constexpr int tileSizePixels = tileWidth * tileHeight;
constexpr int tileSizeBytes = tileSizePixels * sizeof(uint32_t);

constexpr int maxScreenWidthTiles = 40;
constexpr int maxScreenHeightTiles = 40;

constexpr int maxScreenWidthPixels = maxScreenWidthTiles * tileWidth;
constexpr int maxScreenHeightPixels = maxScreenHeightTiles * tileHeight;
constexpr int maxScreenTotalPixels = maxScreenWidthPixels * maxScreenHeightPixels;

constexpr int maxTilemapScreensWidth = 16;
constexpr int maxTilemapScreensHeight = 16;
constexpr int maxTilemapWidthTiles = maxTilemapScreensWidth * maxScreenWidthTiles;
constexpr int maxTilemapHeightTiles = maxTilemapScreensHeight * maxScreenHeightTiles;
constexpr int maxTilemapWidthPixels = maxTilemapWidthTiles * tileWidth;
constexpr int maxTilemapHeightPixels = maxTilemapHeightTiles * tileHeight;
constexpr int maxTilemapSizeTiles =  maxTilemapWidthTiles * maxTilemapHeightTiles;
constexpr int maxTilemapSizePixels = maxTilemapWidthPixels * maxTilemapHeightPixels;

constexpr int numTilePages = 256;
constexpr int tilesPerRow = 16;
constexpr int tilesRowsPerPage = 16; 
constexpr int tilePageWidth = tilesPerRow * tileWidth;
constexpr int tilePageHeight = tilesRowsPerPage * tileHeight;
constexpr int tilesPerPage = tilesPerRow * tilesRowsPerPage;
constexpr int pageSizePixels = tilesPerPage * tileSizePixels;
constexpr int pageSizeBytes = pageSizePixels * sizeof(uint32_t);

constexpr int totalTilesSizePixels = tileSizePixels * tilesPerPage * numTilePages;
constexpr int totalTilesSizeBytes = totalTilesSizePixels * sizeof(uint32_t);
	