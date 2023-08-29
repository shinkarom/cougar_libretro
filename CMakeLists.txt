cmake_minimum_required(VERSION 3.9)
set(LIBNAME cougar_libretro)
project(${LIBNAME} LANGUAGES C CXX VERSION 1.0.1 DESCRIPTION "Cougar fantasy console libretro core")

add_library(libfmsynth STATIC
	vendor/libfmsynth/src/fmsynth.c)
target_include_directories(libfmsynth PUBLIC
	vendor/libfmsynth/include)

add_library(ymfm STATIC
	vendor/ymfm/src/ymfm_adpcm.cpp
	vendor/ymfm/src/ymfm_misc.cpp
	vendor/ymfm/src/ymfm_opl.cpp
	vendor/ymfm/src/ymfm_opm.cpp
	vendor/ymfm/src/ymfm_opn.cpp
	vendor/ymfm/src/ymfm_opq.cpp
	vendor/ymfm/src/ymfm_pcm.cpp
	vendor/ymfm/src/ymfm_ssg.cpp
	)
target_include_directories(ymfm PUBLIC
	vendor/ymfm/src)

set(CMAKE_SHARED_LIBRARY_PREFIX )

add_library(${LIBNAME} SHARED 
    src/libretro.cpp
	src/miniphysfs.cpp
	src/duktape/duktape.c
	src/fs.cpp
    )
set_target_properties(${LIBNAME} PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER src/libretro.h
    LINKER_LANGUAGE CXX
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(${LIBNAME} 
	libfmsynth ymfm)
target_include_directories(${LIBNAME} PUBLIC src)
