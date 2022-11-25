cmake_minimum_required(VERSION 3.23)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)
set(VERSION_TWEAK 0)

project(DespairEngine)

add_subdirectory(Editor)
add_subdirectory(Engine)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DespairEngineEditor)