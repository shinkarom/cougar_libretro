namespace script {
	
	constexpr const char* vblankName = "_LOOP";
	constexpr const char* initName = "_INIT";
	
	constexpr const char* configObjectName = "_CONFIG";
	constexpr const char* mainObjectName = "_MAIN";
	constexpr const char* globalObjectName = "_SYS";
	
	void init();
	void deinit();
	
	bool loadFile(const char* filename);
	void addApi();
	void addApi2();
	
	bool readConfig();
	bool callInit();
	bool callVBlank();
}