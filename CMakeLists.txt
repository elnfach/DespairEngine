cmake_minimum_required(VERSION 3.12)

set(ENGINE_NAME DespairEngine)

add_library(${ENGINE_NAME} STATIC 
	src/DespairEngine/Core.cpp 
	include/DespairEngine/Core.h
)

target_include_directories(${ENGINE_NAME} PUBLIC include)
target_compile_features(${ENGINE_NAME} PUBLIC CXX_STANDARD 22)

add_subdirectory(../libs/GLFW)