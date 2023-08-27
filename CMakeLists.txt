cmake_minimum_required(VERSION 3.9)
set(LIBNAME cougar_libretro)
project(${LIBNAME} LANGUAGES C CXX VERSION 1.0.1 DESCRIPTION "Cougar fantasy console libretro core")

add_library(libfmsynth SHARED
	vendor/libfmsynth/src/fmsynth.c)
target_include_directories(libfmsynth PUBLIC
	vendor/libfmsynth/include)

set(CMAKE_SHARED_LIBRARY_PREFIX )

add_library(${LIBNAME} SHARED 
    src/libretro.cpp
	src/miniphysfs.cpp
    )
set_target_properties(${LIBNAME} PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER src/libretro.h
    LINKER_LANGUAGE CXX
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(${LIBNAME} 
	libfmsynth)
target_include_directories(${LIBNAME} PUBLIC src)
