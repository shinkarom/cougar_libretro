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
	
	int pixelsQuota = windowTotalPixels * 3 / 2;
	
	uint32_t tiles[totalTilesSizeBytes];
	uint16_t tilemap[tilemapSizeTiles];	
	
	int screenWidthTiles = maxScreenWidthTiles;
	int screenHeightTiles = maxScreenHeightTiles;
	
	int drawnPixels;
	
	set_resolution_t resolution_cb;
	
	uint32_t window[maxScreenTotalPixels];
	
	uint32_t** frameBuf;
	
	void loadTiles() {
		memset(tiles, totalTilesSizeBytes, 0);
		
		for(int pageNum = 0; pageNum < numTilePages; pageNum++) {
			char pageName[80];
			sprintf(pageName, "/CHR/CHR_%01X.png", pageNum);
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
				
				auto alpha = (px>>24)&0xFF;
				auto red = (px>>16)&0xFF;
				auto green = (px>>8)&0xFF;
				auto blue = px&0xFF;
				px = (alpha<<24)|(blue<<16)|(green<<8)|red;
				
				auto baseY = pxn / tilePageWidth;
				auto baseX = pxn % tilePageWidth;
				
				auto tileX = (baseX / tileWidth) % tilesPerRow;
				auto tileY = (baseY / tileHeight) % tilesRowsPerPage;
				
				auto pixelX = baseX % tileWidth;
				auto pixelY = baseY % tileHeight;
				
				auto newTilePos = pixelY * tileWidth + pixelX;
				auto newTileIndex = tileY * tilesPerRow + tileX;
				
				auto new_pxn = newTileIndex * tileSizePixels + newTilePos;
				
				tiles[startPos + new_pxn] = px;
			}
			
			delete[] buffer;			
			free(data);
		}		
	}
	
	void init(uint32_t** buffer, set_resolution_t resovalue) {
		frameBuf = buffer;
		resolution_cb = resovalue;
		
		memset(tilemap,tilemapSizeTiles,0);
	}
	
	void deinit() {
		
	}
	
	void beforeProcess() {
		drawnPixels = 0;
	}
	
	void process() {
		
	}
	
	void setResolution(int w, int h) {
		if(w<0 || w>maxScreenWidthTiles/2 || h<0 || h>maxScreenHeightTiles/2) {
			return;
		}
		w=w*2;
		h=h*2;
		resolution_cb(w, h);
		windowWidth = w * tileWidth;
		windowHeight = h * tileHeight;
		windowTotalPixels = windowWidth * windowHeight;
		pixelsQuota = windowTotalPixels * 3 / 2;
		
		screenWidthTiles = w;
		screenHeightTiles = h;

	}
	
	void drawSprite(int index, int x, int y, bool fliph, bool flipv) {
		if(drawnPixels >= pixelsQuota) {
			return;
		}
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
					drawnPixels++;
					if(drawnPixels >= pixelsQuota) {
						return;
					}
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
		if(drawnPixels >= pixelsQuota) {
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
					drawnPixels++;
					if(drawnPixels >= pixelsQuota) {
						return;
					}
				}
			}
		}
	}
	
}