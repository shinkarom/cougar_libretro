#include "script.h"

#include "duktape/duktape.h"
#include "fs.h"

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
	
	bool  callVBlank() {
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
	
}