#include "script.h"

#include "duktape/duktape.h"
#include "fs.h"
#include "apu.h"
#include "ppu.h"
#include "input.h"

#include <iostream>

namespace script {
	
	duk_context* ctx;
	
	void init() {
		ctx = duk_create_heap_default();
	}
	
	void deinit() {
		duk_destroy_heap(ctx);
	}
	
	bool loadFile(const char* filename) {
		char* prgFile;
		auto r = fs::readTextFile(filename, &prgFile, true);
		if(r == -1) {
			return false;
		}
		
		if(duk_peval_string(ctx, prgFile)) {
			std::cout<<"[COUGAR] "<<duk_safe_to_stacktrace(ctx, -1)<<std::endl;
			return false;
		}
		duk_get_global_string(ctx, mainObjectName);
		if(duk_is_undefined(ctx, -1)) {
			std::cout<<"[COUGAR] No object "<<mainObjectName<<std::endl;
			delete[] prgFile;
			return false;
		};
		duk_get_prop_string(ctx, -1, initName);
		if(duk_is_undefined(ctx, -1) || !duk_is_callable(ctx, -1)) {
			std::cout<<"[COUGAR] No function "<<initName<<std::endl;
			delete[] prgFile;
			return false;
		}
		duk_pop(ctx);
		duk_get_prop_string(ctx, -1, vblankName);
		if(duk_is_undefined(ctx, -1) || !duk_is_callable(ctx, -1)) {
			std::cout<<"[COUGAR] No function "<<vblankName<<std::endl;
			delete[] prgFile;
			return false;
		}
		duk_pop(ctx);
		
		delete[] prgFile;
		return true;
	}
	
	bool readConfig() {
		int w, h, sw, sh, b;
		if(!duk_get_global_string(ctx, configObjectName) || 
			!duk_is_object(ctx, -1)) {
			std::cout<<"[COUGAR] No object "<<configObjectName<<std::endl;
			return false;
		}
		duk_get_prop_string(ctx, -1, "WIDTH");
		if(duk_is_number(ctx, -1)) {
			w = duk_get_int(ctx, -1);
			if(w < 1 || w > maxScreenWidthTiles)  {
				std::cout<<"[COUGAR] Width must be from 1 to "<<maxScreenWidthTiles<<std::endl;
				return false;
			}
		} else {
			std::cout<<"[COUGAR] No width in config."<<std::endl;
			return false;
		}
		duk_pop(ctx);
		duk_get_prop_string(ctx, -1, "HEIGHT");
		if(duk_is_number(ctx, -1)) {
			h = duk_get_int(ctx, -1);
			if(h < 1 || h > maxScreenHeightTiles)  {
				std::cout<<"[COUGAR] Height must be from 1 to "<<maxScreenHeightTiles<<std::endl;
				return false;
			}
		} else {
			std::cout<<"[COUGAR] No height in config."<<std::endl;
			return false;
		}
		duk_pop(ctx);
		
		duk_pop(ctx);
		ppu::setResolution(w, h);
		return true;
	}
	
	bool callInit() {
		duk_get_global_string(ctx, mainObjectName);
		duk_get_prop_string(ctx, -1, initName);
		if(duk_is_callable(ctx, -1) && duk_pcall(ctx, 0)){
			std::cout<<"[COUGAR] "<<duk_safe_to_stacktrace(ctx, -1)<<std::endl;
			return false;
		}
		duk_pop_2(ctx);
		return true;
	}
	
	bool callVBlank() {
		duk_get_global_string(ctx, mainObjectName);
		duk_get_prop_string(ctx, -1, vblankName);
		if(duk_is_callable(ctx, -1) && duk_pcall(ctx, 0)){
			std::cout<<"[COUGAR] "<<duk_safe_to_stacktrace(ctx, -1)<<std::endl;
			return false;
		}
		duk_pop_2(ctx);
		return true;
	}
	
	
	duk_ret_t apiClearScreen(duk_context* ctx) {
		auto color = (uint32_t)duk_require_number(ctx, -1);
		ppu::clearScreen(color);
		return 0;
	}
	
	duk_ret_t apiDrawSprite(duk_context* ctx) {
		auto index = (uint32_t)duk_require_number(ctx, -5);
		auto x = duk_require_int(ctx, -4);
		auto y = duk_require_int(ctx, -3);
		auto fliph = duk_require_boolean(ctx, -2);
		auto flipv = duk_require_boolean(ctx, -1);
		ppu::drawSprite(index, x, y, fliph, flipv);
		return 0;
	}
	
	duk_ret_t apiButtonPressed(duk_context* ctx) {
		auto index = duk_require_int(ctx, -1);
		if(index <0 || index >=12 ||!input::isButtonPressed(index)) {
			duk_push_boolean(ctx, false);
		} else {
			duk_push_boolean(ctx, true);
		}
		return 1;
	}
	
	duk_ret_t apiButtonReleased(duk_context* ctx) {
		auto index = duk_require_int(ctx, -1);
		if(index <0 || index >=12 ||!input::isButtonReleased(index)) {
			duk_push_boolean(ctx, false);
		} else {
			duk_push_boolean(ctx, true);
		}
		return 1;
	}
	
	duk_ret_t apiTilemapTile(duk_context* ctx) {
		int numArgs = duk_get_top(ctx); // Get the number of arguments passed

		if (numArgs == 2) {
			// Two arguments: get tile
			auto w = duk_require_int(ctx, -2); // Get the width
			auto h = duk_require_int(ctx, -1); // Get the height
			auto t = ppu::getTilemapTile(w, h); // Retrieve the tile value
			duk_push_int(ctx, t); // Push the tile value onto the stack
			return 1; // Return the tile value
		} 
		else if (numArgs == 3) {
			// Three arguments: set tile
			auto w = duk_require_int(ctx, -3); // Get the width
			auto h = duk_require_int(ctx, -2); // Get the height
			auto value = duk_require_int(ctx, -1); // Get the tile value to set
			ppu::setTilemapTile(w, h, value); // Set the tile value
			return 0; // No return value needed for a setter
		} 
		else {
			// Throw an error if the number of arguments is not 2 or 3
			duk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, "Expected 2 or 3 arguments, got %d", numArgs);
			return DUK_RET_ERROR; // Return an error
		}
	}


	duk_ret_t apiWriteReg(duk_context* ctx) {
		auto reg = duk_require_int(ctx, -2);
		auto value = duk_require_int(ctx, -1);
		if(reg>0 || reg < UINT16_MAX || value > 0 && value < UINT8_MAX) {
			apu::writeReg(reg, value);
		}
		return 0;
	}
	
	duk_ret_t apiDrawTilemap(duk_context* ctx) {
		auto sx = duk_require_int(ctx, -6);
		auto sy = duk_require_int(ctx, -5);
		auto x = duk_require_int(ctx, -4);
		auto y = duk_require_int(ctx, -3);
		auto w = duk_require_int(ctx, -2);
		auto h = duk_require_int(ctx, -1);
		ppu::drawTilemap(sx, sy, x, y, w, h);
		return 0;
	}
		
	void addApi() {
		const duk_function_list_entry cougarApi[] = {
			{"TILE", apiTilemapTile, DUK_VARARGS},
			{"REG", apiWriteReg, 2},
			{nullptr, nullptr, 0}
		};
		
		duk_push_global_object(ctx);
		duk_push_object(ctx);
		duk_put_prop_string(ctx, -2, globalObjectName);
		duk_get_prop_string(ctx, -1, globalObjectName);
		duk_put_function_list(ctx, -1, cougarApi);
		duk_pop(ctx);
	}
	
	void addApi2() {
		const duk_function_list_entry cougarApi[] = {
			{"CLEAR", apiClearScreen, 1},
			{"SPRITE", apiDrawSprite, 5},
			{"PRESSED", apiButtonPressed, 1},
			{"RELEASED", apiButtonReleased, 1},
			{"MAP", apiDrawTilemap, 6},
			{nullptr, nullptr, 0}
		};
		
		duk_get_global_string(ctx, globalObjectName);
		duk_put_function_list(ctx, -1, cougarApi);
		duk_pop(ctx);
	}
}