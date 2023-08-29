#include "fs.h"
#include "miniphysfs.h"


namespace fs {
	
	PHYSFS_File* handles[32];
	
	void init() {
		for (int i = 0; i < 32; i++) {
			handles[i] = nullptr;
		}
		PHYSFS_init(nullptr);
	}
	
	void deinit() {
		PHYSFS_deinit();
	}
	
	bool load(const char* filename) {
		return PHYSFS_mount(filename, "/", 0) != 0;
	}
	
	bool exists(const char* filename) {
		return PHYSFS_exists(filename);
	}
	
	int64_t size(const char* filename) {
		auto h = open(filename);
		if(h == -1) {
			return -1;
		} else {
			auto l = PHYSFS_fileLength(handles[h]);
			close(h);
			return l;
		}
	}
	
	int open(const char* filename) {
		auto handle = PHYSFS_openRead(filename);
		if(!handle) {
			return -1;
		} else {
			for(int i = 0; i< 32; i++) {
				if (!handles[i]) {
					handles[i] = handle;
					return i;
				}
			}
			return -1;
		}
	}
	
	void close(int handle) {
		if(handle >= 32 || handle < 0 || !handles[handle]) {
			return;
		}
		PHYSFS_close(handles[handle]);
	}
	
	int64_t tell(int handle) {
		if(handle >= 32 || handle < 0 || !handles[handle]) {
			return -1;
		}
		
		return PHYSFS_tell(handles[handle]);
	}
	
	bool seek(int handle, uint64_t pos) {
		if(handle >= 32 || handle < 0 || !handles[handle]) {
			return -1;
		}
		
		return PHYSFS_seek(handles[handle], pos) != 0;
		
	}
	
	int64_t readBytes(int handle, void* buffer, uint64_t len) {
		if(handle >= 32 || handle < 0 || !handles[handle]) {
			return -1;
		}
		
		return PHYSFS_readBytes(handles[handle], buffer, len);
	}
	
	int64_t readBinaryFile(const char* filename, void* buffer, bool doMAlloc) {
		auto handle = open(filename);
		if(handle == -1) {
			return -1;
		}
		auto size = PHYSFS_fileLength(handles[handle]);
		if(size == -1) {
			return -1;
		}
		
		if (doMAlloc) {
			buffer = new char[size];
		}
		
		seek(handle, 0);
		return readBytes(handle, buffer, size);
	}
	
	int64_t readTextFile(const char* filename, void* buffer, bool doMAlloc) {
		auto handle = open(filename);
		if(handle == -1) {
			return -1;
		}
		auto size = PHYSFS_fileLength(handles[handle]);
		if(size == -1) {
			return -1;
		}
		
		if (doMAlloc) {
			buffer = new char[size+1]();
		}
		
		seek(handle, 0);
		auto r = readBytes(handle, buffer, size);
		((char*)buffer)[size] = '\0';
		return r;
	}
	
}