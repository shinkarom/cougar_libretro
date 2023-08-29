#include "fs.h"
#include "miniphysfs.h"

namespace fs {
	
	void init() {
		PHYSFS_init(nullptr);
	}
	
	void deinit() {
		PHYSFS_deinit();
	}
	
	bool load(const char* filename) {
		return PHYSFS_mount(filename, "/", 0) != 0;
	}
	
}