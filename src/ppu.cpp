#include "ppu.h"
#include "fs.h"
#include "common.h"

#include <cstring>
#include <cstdio>
#include <iostream>
#include <stb_image.h>

namespace ppu {
	
	int windowWidth = maxScreenWidthPixels;
	int windowHeight = maxScreenHeightPixels;
	int windowTotalPixels = windowWidth * windowHeight;
	
	uint32_t tiles[totalTilesSizeBytes];
	uint16_t tilemap[maxTilemapSizeTiles];	
	
	int screenWidthTiles = maxScreenWidthTiles;
	int screenHeightTiles = maxScreenHeightTiles;
	int tilemapWidthTiles = tilemapScreensWidth * screenWidthTiles;
	int tilemapHeightTiles = tilemapScreensHeight * screenHeightTiles;
	int tilemapWidthPixels = tilemapWidthTiles * tileWidth;
	int tilemapHeightPixels = tilemapHeightTiles * tileHeight;
	int tilemapSizeTiles =  tilemapWidthTiles * tilemapHeightTiles;
	int tilemapSizePixels = tilemapWidthPixels * tilemapHeightPixels;
	
	set_resolution_t resolution_cb;
	
	uint32_t window[maxScreenTotalPixels];
	
	uint32_t** frameBuf;
	
	void loadTiles() {
		memset(tiles, totalTilesSizeBytes, 0);
		
		for(int pageNum = 0; pageNum < numTilePages; pageNum++) {
			char pageName[80];
			sprintf(pageName, "/CHR/CHR_%02X.png", pageNum);
			auto fileSize = fs::size(pageName);		
			int loadSize = (fileSize > pageSizeBytes) ? pageSizeBytes : fileSize;
			
			auto startPos = pageSizePixels * pageNum;
						
			char* buffer;
			auto r = fs::readBinaryFile(pageName, &buffer, true);
			if (r == -1) continue;
			
			// load the file
			int x, y, n, ok;
			ok = stbi_info_from_memory((stbi_uc *)buffer, r, &x, &y, &n);
			if(x != tilePageWidth && y != tilePageHeight) {
				std::cout<<"[COUGAR] "<<pageName<<" is not of required dimensions."<<std::endl;
				delete[] buffer;
				continue;
			}
			
			auto data = stbi_load_from_memory((stbi_uc *)buffer, r, &x, &y, &n, 4);
			if(data == nullptr) {
				delete[] buffer;
				continue;
			}
			
			std::cout<<"[COUGAR] Will load from "<<pageName<<std::endl;
			auto pixels = (uint32_t*)data;
	
			for(auto pxn = 0; pxn < pageSizePixels; pxn++) {
				auto px = pixels[pxn];
				
				auto baseY = pxn / tilePageWidth;
				auto baseX = pxn % tilePageWidth;
				
				auto tileX = (baseX / tileWidth) % tilesPerRow;
				auto tileY = (baseY / tileHeight) % tilesRowsPerPage;
				
				auto pixelX = baseX % tileWidth;
				auto pixelY = baseY % tileHeight;
				
				auto newTilePos = pixelY * tileWidth + pixelX;
				auto newTileIndex = tileY * tilesPerPage + tileX;
				
				auto new_pxn = newTileIndex * tileSizePixels + newTilePos;
				
				tiles[pageSizePixels*pageNum + new_pxn] = px;
			}
			
			delete[] buffer;			
			free(data);
		}		
	}
	
	void init(uint32_t** buffer, set_resolution_t resovalue) {
		frameBuf = buffer;
		resolution_cb = resovalue;
		
		memset(tilemap,maxTilemapSizeTiles,0);
	}
	
	void deinit() {
		
	}
	
	void process() {
		
	}
	
	void setResolution(int w, int h) {
		if(w<0 || w>maxScreenWidthPixels || h<0 || h>maxScreenHeightPixels) {
			return;
		}
		if(w%tileWidth != 0 || h%tileHeight != 0) {
			return;
		}
		resolution_cb(w, h);
		windowWidth = w;
		windowHeight = h;
		windowTotalPixels = windowWidth * windowHeight;
		
		screenWidthTiles = w / tileWidth;
		screenHeightTiles = h / tileHeight;
		tilemapWidthTiles = tilemapScreensWidth * screenWidthTiles;
		tilemapHeightTiles = tilemapScreensHeight * screenHeightTiles;
		tilemapWidthPixels = tilemapWidthTiles * tileWidth;
		tilemapHeightPixels = tilemapHeightTiles * tileHeight;
		tilemapSizeTiles =  tilemapWidthTiles * tilemapHeightTiles;
		tilemapSizePixels = tilemapWidthPixels * tilemapHeightPixels;

	}
	
	void drawSprite(int index, int x, int y, bool fliph, bool flipv) {
		for(int yy = 0; yy < tileHeight; yy++) {
			if(y+yy < 0) {
				continue;
			} else if(y+yy >= windowHeight) {
				break;
			}
			for(int xx = 0; xx < tileWidth; xx++) {
				if(x+xx < 0) {
					continue;
				} else if(x+xx >= windowWidth) {
					break;
				}
				int pixelIndex = (y+yy)*windowWidth+(x+xx);
				int xFactor = fliph ? tileWidth - xx - 1 : xx;
				int yFactor = flipv ? tileHeight - yy - 1 : yy;
				int tileStart = index * tileSizePixels +yFactor*tileWidth+xFactor;
				if(tiles[tileStart] & 0xFF000000) {
					(*frameBuf)[pixelIndex] = tiles[tileStart];
				}	
			}
		}
		
	}
	
	void clearScreen(int32_t color) {
		for(int i = 0; i<windowTotalPixels; i++) {
			(*frameBuf)[i] = color;
		}
	}
	
	uint16_t getTilemapTile(int w, int h) {
		if(w < 0 || w >= tilemapWidthTiles || h < 0 || h >= tilemapHeightTiles) {
			return 0;
		}
		auto index = h*tilemapWidthTiles + w;
		return tilemap[index];
	}
	
	void setTilemapTile(int w, int h, uint16_t value) {
		if(w < 0 || w >= tilemapWidthTiles || h < 0 || h >= tilemapHeightTiles) {
			return;
		}
		auto index = h*tilemapWidthTiles + w;
		tilemap[index] = value;
	}
	
	void drawTilemap(int sx, int sy, int x, int y, int w, int h) {
		if(sx < 0 || sx >= tilemapWidthTiles || sy < 0 || sy >= tilemapHeightTiles) {
			return;
		}
		//if(x < 0 || x >= windowWidth || y < 0 || y>= windowHeight) {
		//	return;
		//}
		//if(w < 0 || w >= windowWidth || h < 0 || h >= windowHeight) {
		//	return;
		//}
		for(int wi = 0; wi < w; wi++) {
			auto screenX = x+wi;
			if(screenX < 0) {
				continue;
			} else if(screenX >= windowWidth) {
				break;
			}
			
			auto tilemapX = (sx + wi) % tilemapWidthPixels;
			auto tileX = tilemapX / tileWidth;
			auto offsetX = tilemapX % tileWidth;
			
			for(int hi = 0; hi < h; hi++) {
				auto screenY = y+hi;
				if(screenY < 0) {
					continue;
				} else if(screenY >= windowHeight) {
					break;
				}
				
				auto tilemapY = (sy + hi) % tilemapHeightPixels;
				auto tileY = tilemapY / tileHeight;
				auto offsetY = tilemapY % tileHeight;
				
				auto pixelPos = screenY * windowWidth + screenX;
				
				auto tilemapIndex = tileY * tilemapWidthTiles + tileX;
				auto tileIndex = tilemap[tilemapIndex];
				
				auto offsetIndex = offsetY * tileWidth + offsetX;
				auto pixelIndex = tileIndex * tileSizePixels + offsetIndex;
				uint32_t pixel = tiles[pixelIndex];
				// draw it
				if(pixel & 0xFF000000) {
					(*frameBuf)[pixelPos] = pixel;
				}
			}
		}
	}
	
}