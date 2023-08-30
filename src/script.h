namespace script {
	
	constexpr const char* vblankName = "_LOOP";
	constexpr const char* initName = "_INIT";
	
	void init();
	void deinit();
	
	bool loadFile(const char* filename);
	
	bool callInit();
	bool callVBlank();
}