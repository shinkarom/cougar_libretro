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
		duk_get_prop_string(ctx, -1, "SCREENSWIDTH");
		if(duk_is_number(ctx, -1)) {
			sw = duk_get_int(ctx, -1);
			if(sw < 1 || w > maxTilemapScreensWidth)  {
				std::cout<<"[COUGAR] Screens width must be from 1 to "<<maxTilemapScreensWidth<<std::endl;
				return false;
			}
		} else {
			std::cout<<"[COUGAR] No screens width in config."<<std::endl;
			return false;
		}
		duk_pop(ctx);
		duk_get_prop_string(ctx, -1, "SCREENSHEIGHT");
		if(duk_is_number(ctx, -1)) {
			sh = duk_get_int(ctx, -1);
			if(h < 1 || h > maxTilemapScreensHeight)  {
				std::cout<<"[COUGAR] Screens height must be from 1 to "<<maxTilemapScreensHeight<<std::endl;
				return false;
			}
		} else {
			std::cout<<"[COUGAR] No screens height in config."<<std::endl;
			return false;
		}
		duk_pop(ctx);
		
		duk_pop(ctx);
		ppu::setResolution(w, h, sw, sh);
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
	
	duk_ret_t apiLoadTrack(duk_context *ctx) {
		auto playerNum = duk_require_int(ctx, -2);
		int trackNum = duk_require_int(ctx, -1);
		if(trackNum < 0 || trackNum >= maxAudioTracks) {
			return 0;
		}
		apu::loadFile(playerNum, trackNum);
		return 0;
	}
	
	duk_ret_t apiPlayTrack(duk_context *ctx) {
		auto playerNum = duk_require_int(ctx, -2);
		int trackNum = duk_require_int(ctx, -1);
		if(trackNum < 0 || trackNum >= maxAudioTracks) {
			return 0;
		}
		apu::loadFile(playerNum, trackNum);
		apu::setPlaying(playerNum, true);
		return 0;
	}
	
	duk_ret_t apiGetVolume(duk_context* ctx) {
		auto playerNum = duk_require_int(ctx, -1);
		int t = apu::getVolume(playerNum);
		duk_push_int(ctx, t);
		return 1;
	}
	
	duk_ret_t apiSetVolume(duk_context* ctx) {
		auto playerNum = duk_require_int(ctx, -2);
		auto value = duk_require_int(ctx, -1);
		apu::setVolume(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiIsPlaying(duk_context* ctx) {
		auto playerNum = duk_require_int(ctx, -1);
		auto t = apu::isPlaying(playerNum);
		duk_push_boolean(ctx, t);
		return 1;
	}
	
	duk_ret_t apiSetPlaying(duk_context* ctx) {
		auto playerNum = duk_require_int(ctx, -2);
		auto value = duk_require_boolean(ctx, -1);
		apu::setPlaying(playerNum, value);
		return 0;
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
	
	duk_ret_t apiSetTrackPosition(duk_context* ctx) {
		auto playerNum = duk_require_int(ctx, -2);
		auto value = duk_require_int(ctx, -1);
		apu::setPosition(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiGetTrackPosition(duk_context* ctx) {
		auto playerNum = duk_require_int(ctx, -1);
		auto t = apu::getPosition(playerNum);
		duk_push_int(ctx, t);
		return 1;
	}
	
	duk_ret_t apiTrackLength(duk_context*ctx) {
		auto playerNum = duk_require_int(ctx, -1);
		auto l = apu::trackLength(playerNum);
		duk_push_int(ctx, l);
		return 1;
	}
	
	duk_ret_t apiIsLooping(duk_context* ctx) {
		auto playerNum = duk_require_int(ctx, -1);
		auto t = apu::isLooping(playerNum);
		duk_push_boolean(ctx, t);
		return 1;
	}
	
	duk_ret_t apiSetLooping(duk_context* ctx) {
		auto playerNum = duk_require_int(ctx, -2);
		auto value = duk_to_boolean(ctx, -1);
		apu::setLooping(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiGetTilemapTile(duk_context* ctx) {
		auto w = duk_require_int(ctx, -2);
		auto h = duk_require_int(ctx, -1);
		auto t = ppu::getTilemapTile(w, h);
		duk_push_int(ctx, t);
		return 1;
	}
	
	duk_ret_t apiSetTilemapTile(duk_context* ctx) {
		auto w = duk_require_int(ctx, -3);
		auto h = duk_require_int(ctx, -2);
		auto value = duk_require_int(ctx, -1);
		ppu::setTilemapTile(w, h, value);
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
			{"LOADTRACK", apiLoadTrack, 2},
			{"GETVOLUME", apiGetVolume, 1},
			{"SETVOLUME", apiSetVolume, 2},
			{"GETTRACKPOSITION", apiGetTrackPosition, 1},
			{"SETTRACKPOSITION", apiSetTrackPosition, 2},
			{"TRACKLENGTH", apiTrackLength, 1},
			{"ISLOOPING", apiIsLooping, 1},
			{"SETLOOPING", apiSetLooping, 2},
			{"GETTILEMAPTILE", apiGetTilemapTile, 2},
			{"SETTILEMAPTILE", apiSetTilemapTile, 3},
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
			{"PLAYTRACK", apiPlayTrack, 2},
			{"ISPLAYING", apiIsPlaying, 1},
			{"SETPLAYING", apiSetPlaying, 2},
			{"CLEAR", apiClearScreen, 1},
			{"SPRITE", apiDrawSprite, 5},
			{"BUTTONPRESSED", apiButtonPressed, 1},
			{"BUTTONRELEASED", apiButtonReleased, 1},
			{"DRAWTILEMAP", apiDrawTilemap, 6},
			{nullptr, nullptr, 0}
		};
		
		duk_get_global_string(ctx, globalObjectName);
		duk_put_function_list(ctx, -1, cougarApi);
		duk_pop(ctx);
	}
}