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
		
		duk_eval_string_noresult(ctx, prgFile);
		
		delete[] prgFile;
		return true;
	}
	
}