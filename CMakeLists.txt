cmake_minimum_required(VERSION 3.9)
set(LIBNAME cougar_libretro)
set(CMAKE_CXX_STANDARD 23)
project(${LIBNAME} LANGUAGES C CXX VERSION 1.0.1 DESCRIPTION "Cougar fantasy console libretro core")

set(CMAKE_SHARED_LIBRARY_PREFIX )

add_library(${LIBNAME} SHARED 
    src/libretro.cpp
	src/miniphysfs.cpp
	src/duktape/duktape.c
	src/stb_vorbis.c
	src/stb_image.c
	src/fs.cpp
	src/script.cpp
	src/input.cpp
	src/ppu.cpp
	src/apu.cpp
	src/audioplayer.cpp
    )
set_target_properties(${LIBNAME} PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER src/libretro.h
    LINKER_LANGUAGE CXX
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#target_link_libraries(${LIBNAME} )
target_include_directories(${LIBNAME} PUBLIC src)
