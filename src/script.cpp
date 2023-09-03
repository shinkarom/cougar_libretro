#include "script.h"

#include "duktape/duktape.h"
#include "fs.h"
#include "apu.h"

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
	
	duk_ret_t apiPlayTrack(duk_context *ctx) {
		//ensure 2 arguments via duk_get_top
		const char* fileName = duk_get_string(ctx, -1);
		auto playerNum = duk_get_int(ctx, -2);
		apu::playFile(playerNum, fileName);
		return 0;
	}
	
	duk_ret_t apiSetVolume(duk_context* ctx) {
		//ensure 2 arguments via duk_get_top
		auto value = duk_get_int(ctx, -1);
		auto playerNum = duk_get_int(ctx, -2);
		apu::setVolume(playerNum, value);
		return 0;
	}
	
	duk_ret_t apiPlay(duk_context* ctx) {
		auto playerNum = duk_get_int(ctx, -2);
		apu::play(playerNum);
		return 0;
	}
	
	duk_ret_t apiStop(duk_context* ctx) {
		auto playerNum = duk_get_int(ctx, -2);
		apu::stop(playerNum);
		return 0;
	}
	
	const duk_function_list_entry cougarApi[] = {
		{"playTrack", apiPlayTrack, 2},
		{"setVolume", apiSetVolume, 2},
		{"play", apiPlay, 1},
		{"stop", apiStop, 1},
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