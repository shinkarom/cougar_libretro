namespace script {
	
	constexpr const char* vblankName = "_LOOP";
	
	void init();
	void deinit();
	
	bool loadFile(const char* filename);
	
	bool  callVBlank();
}