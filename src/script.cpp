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
		delete[] prgFile;
		return true;
	}
	
	bool callInit() {
		duk_push_global_object(ctx);
		duk_get_prop_string(ctx, -1, initName);
		if(duk_is_callable(ctx, -1) && duk_pcall(ctx, 0)){
			std::cout<<"[COUGAR] "<<duk_safe_to_stacktrace(ctx, -1)<<std::endl;
			return false;
		}
		duk_pop(ctx);
		duk_pop(ctx);
		return true;
	}
	
	bool callVBlank() {
		duk_push_global_object(ctx);
		duk_get_prop_string(ctx, -1, vblankName);
		if(duk_is_callable(ctx, -1) && duk_pcall(ctx, 0)){
			std::cout<<"[COUGAR] "<<duk_safe_to_stacktrace(ctx, -1)<<std::endl;
			return false;
		}
		duk_pop(ctx);
		duk_pop(ctx);
		return true;
	}
	
	duk_ret_t apiLoadTrack(duk_context *ctx) {
		//ensure 2 arguments via duk_get_top
		const char* fileName = duk_get_string(ctx, -1);
		auto playerNum = duk_get_int(ctx, -2);
		apu::loadFile(playerNum, fileName);
		return 0;
	}
	
	duk_ret_t apiPlayTrack(duk_context *ctx) {
		//ensure 2 arguments via duk_get_top
		const char* fileName = duk_get_string(ctx, -1);
		auto playerNum = duk_get_int(ctx, -2);
		apu::loadFile(playerNum, fileName);
		apu::setPlaying(playerNum, true);
		return 0;
	}
	
	duk_ret_t apiSetVolume(duk_context* ctx) {
		auto value = duk_get_int(ctx, -1);
		auto playerNum = duk_get_int(ctx, -2);
		apu::setVolume(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiIsPlaying(duk_context* ctx) {
		auto playerNum = duk_get_int(ctx, -1);
		auto t = apu::isPlaying(playerNum);
		duk_push_boolean(ctx, t);
		return 1;
	}
	
	duk_ret_t apiSetPlaying(duk_context* ctx) {
		auto playerNum = duk_get_int(ctx, -2);
		auto value = duk_get_boolean(ctx, -1);
		apu::setPlaying(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiSetResolution(duk_context* ctx) {
		auto h = duk_get_int(ctx, -1);
		auto w = duk_get_int(ctx, -2);
		ppu::setResolution(w, h);
		return 0;
	}
	
	duk_ret_t apiClearScreen(duk_context* ctx) {
		auto color = duk_to_uint32(ctx, -1);
		ppu::clearScreen(color);
		return 0;
	}
	
	duk_ret_t apiDrawSprite(duk_context* ctx) {
		auto index = duk_to_uint32(ctx, -5);
		auto x = duk_to_int(ctx, -4);
		auto y = duk_to_int(ctx, -3);
		auto fliph = duk_to_boolean(ctx, -2);
		auto flipv = duk_to_boolean(ctx, -1);
		ppu::drawSprite(index, x, y, fliph, flipv);
		return 0;
	}
	
	duk_ret_t apiButtonPressed(duk_context* ctx) {
		auto index = duk_to_int(ctx, -1);
		if(index <0 || index >=12 ||!input::isButtonPressed(index)) {
			duk_push_boolean(ctx, false);
		} else {
			duk_push_boolean(ctx, true);
		}
		return 1;
	}
	
	duk_ret_t apiButtonReleased(duk_context* ctx) {
		auto index = duk_to_int(ctx, -1);
		if(index <0 || index >=12 ||!input::isButtonReleased(index)) {
			duk_push_boolean(ctx, false);
		} else {
			duk_push_boolean(ctx, true);
		}
		return 1;
	}
	
	duk_ret_t apiSetTrackPosition(duk_context* ctx) {
		auto playerNum = duk_to_int(ctx, -2);
		auto value = duk_to_int(ctx, -1);
		apu::setPosition(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiGetTrackPosition(duk_context* ctx) {
		auto playerNum = duk_to_int(ctx, -1);
		auto t = apu::getPosition(playerNum);
		duk_push_int(ctx, t);
		return 1;
	}
	
	duk_ret_t apiTrackLength(duk_context*ctx) {
		auto playerNum = duk_to_int(ctx, -1);
		auto l = apu::trackLength(playerNum);
		duk_push_int(ctx, l);
		return 1;
	}
	
	duk_ret_t apiIsLooping(duk_context* ctx) {
		auto playerNum = duk_to_int(ctx, -1);
		auto t = apu::isLooping(playerNum);
		duk_push_boolean(ctx, t);
		return 1;
	}
	
	duk_ret_t apiGetLoopStart(duk_context* ctx) {
		auto playerNum = duk_to_int(ctx, -1);
		auto t = apu::getLoopStart(playerNum);
		duk_push_int(ctx, t);
		return 1;
	}
	
	duk_ret_t apiGetLoopEnd(duk_context* ctx) {
		auto playerNum = duk_to_int(ctx, -1);
		auto t = apu::getLoopEnd(playerNum);
		duk_push_int(ctx, t);
		return 1;
	}
	
	duk_ret_t apiSetLooping(duk_context* ctx) {
		auto playerNum = duk_to_int(ctx, -2);
		auto value = duk_to_boolean(ctx, -1);
		apu::setLooping(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiSetLoopStart(duk_context* ctx) {
		auto playerNum = duk_to_int(ctx, -2);
		auto value = duk_to_int32(ctx, -1);
		apu::setLoopStart(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiSetLoopEnd(duk_context* ctx) {
		auto playerNum = duk_to_int(ctx, -2);
		auto value = duk_to_int32(ctx, -1);
		apu::setLoopEnd(playerNum, value);
		return 0;
	}
	
	const duk_function_list_entry cougarApi[] = {
		{"loadTrack", apiLoadTrack, 2},
		{"playTrack", apiPlayTrack, 2},
		{"volume", apiSetVolume, 2},
		{"isPlaying", apiIsPlaying, 1},
		{"setPlaying", apiSetPlaying, 2},
		{"resolution", apiSetResolution, 2},
		{"clear", apiClearScreen, 1},
		{"sprite", apiDrawSprite, 5},
		{"buttonPressed", apiButtonPressed, 1},
		{"buttonReleased", apiButtonReleased, 1},
		{"getTrackPosition", apiGetTrackPosition, 1},
		{"setTrackPosition", apiSetTrackPosition, 2},
		{"trackLength", apiTrackLength, 1},
		{"isLooping", apiIsLooping, 1},
		{"setLooping", apiSetLooping, 2},
		{"getLoopStart", apiGetLoopStart, 1},
		{"setLoopStart", apiSetLoopStart, 2},
		{"getLoopEnd", apiGetLoopEnd, 1},
		{"setLoopEnd", apiSetLoopEnd, 2},		
		{nullptr, nullptr, 0}
	};
	
	void addApi() {
		duk_push_global_object(ctx);
		duk_push_object(ctx);
		duk_put_prop_string(ctx, -2, globalObjectName);
		duk_get_prop_string(ctx, -1, globalObjectName);
		duk_put_function_list(ctx, -1, cougarApi);
		duk_pop(ctx);
	}
}