namespace script {
	
	constexpr const char* vblankName = "_LOOP";
	constexpr const char* initName = "_INIT";
	
	constexpr const char* globalObjectName = "_SYS";
	
	void init();
	void deinit();
	
	bool loadFile(const char* filename);
	void addApi();
	
	bool callInit();
	bool callVBlank();
}