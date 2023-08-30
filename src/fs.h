#include <cstdint>

namespace fs {
	
	void init();
	void deinit();
	bool load(const char* filename);
	
	bool exists(const char* filename);
	int64_t size(const char* fileame);
	int open(const char* filename);
	void close(int handle);
	int64_t tell(int handle);
	bool seek(int handle, uint64_t pos);
	int64_t readBytes(int handle, char* buffer, uint64_t len);
	
	int64_t readBinaryFile(const char* filename, char** buffer, bool doMAlloc = true);
	int64_t readTextFile(const char* filename, char** buffer, bool doMAlloc = true);
	
	void enumerate();
}