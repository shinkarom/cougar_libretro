#include "ppu.h"
#include "fs.h"

#include <cstring>
#include <cstdio>
#include <iostream>

namespace ppu {
	
	uint32_t tiles[totalTilesSizeBytes];
	tilemap_t tilemaps[numTilemaps];
	
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
	
	void init(uint32_t** buffer) {
		frameBuf = buffer;
		
		for(int i = 0; i < numTilemaps; i++) {
			memset(tilemaps[i],totalTilemapSizeTiles,0);
		}
	}
	
	void deinit() {
		
	}
	
	void process() {
		
	}
	
}