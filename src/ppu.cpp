#include "ppu.h"
#include "fs.h"
#include "common.h"

#include <cstring>
#include <cstdio>
#include <iostream>

namespace ppu {
	
	int windowWidth = maxScreenWidthPixels;
	int windowHeight = maxScreenHeightPixels;
	int windowTotalPixels = windowWidth * windowHeight;
	
	uint32_t tiles[totalTilesSizeBytes];
	tilemap_t tilemaps[numTilemaps];
	
	set_resolution_t resolution_cb;
	
	uint32_t window[maxScreenTotalPixels];
	
	uint32_t** frameBuf;
	
	void loadTiles() {
		memset(tiles, totalTilesSizeBytes, 0);
		
		for(int pageNum = 0; pageNum < numTilePages; pageNum++) {
			char pageName[80];
			sprintf(pageName, "/CHR/CHR_%02X", pageNum);
			auto fileSize = fs::size(pageName);		
			int loadSize = (fileSize > pageSizeBytes) ? pageSizeBytes : fileSize;
			
			auto startPos = pageSizePixels * pageNum;
						
			char* buffer;
			auto r = fs::readBinaryFile(pageName, &buffer, true);
			if (r == -1) continue;
			std::cout<<"[COUGAR] "<<pageNum<<" "<<r<<std::endl;
			memcpy(tiles+startPos, buffer, loadSize*sizeof(uint32_t));
			delete[] buffer;
		}		
	}
	
	void init(uint32_t** buffer, set_resolution_t resovalue) {
		frameBuf = buffer;
		resolution_cb = resovalue;
		
		for(int i = 0; i < numTilemaps; i++) {
			memset(tilemaps[i],totalTilemapSizeTiles,0);
		}
	}
	
	void deinit() {
		
	}
	
	void process() {
		
	}
	
	void setResolution(int w, int h) {
		if(w<0 || w>maxScreenWidthPixels || h<0 || h>maxScreenHeightPixels) {
			return;
		}
		resolution_cb(w, h);
		windowWidth = w;
		windowHeight = h;
		windowTotalPixels = windowWidth * windowHeight;
	}
	
	void drawSprite(int index, int x, int y, bool fliph, bool flipv) {
		for(int yy = 0; yy < tileHeight; yy++) {
			if(y+yy >= windowHeight) {
				break;
			}
			for(int xx = 0; xx < tileWidth; xx++) {
				if(x+xx >= windowWidth) {
					break;
				}
				int pixelIndex = (y+yy)*windowWidth+(x+xx);
				int xFactor = fliph ? tileWidth - xx - 1 : xx;
				int yFactor = flipv ? tileHeight - yy - 1 : yy;
				int tileStart = index * tileSizePixels +yFactor*tileWidth+xFactor;
				(*frameBuf)[pixelIndex] = tiles[tileStart];
			}
		}
		
	}
	
	void clearScreen(int32_t color) {
		for(int i = 0; i<windowTotalPixels; i++) {
			(*frameBuf)[i] = color;
		}
	}
	
}