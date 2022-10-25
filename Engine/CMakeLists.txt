cmake_minimum_required(VERSION 3.23)

set(ENGINE_NAME DespairEngine)

set(ENGINE_PUBLIC_INCLUDES
	include/DespairEngine/Core.h
	include/DespairEngine/Event.h
)

set(ENGINE_PRIVATE_INCLUDES
	include/DespairEngine/Debugger.h
	include/DespairEngine/Window.h
)

set(ENGINE_PRIVATE_SOURCES
	src/DespairEngine/Core.cpp
	src/DespairEngine/Window.cpp
)

add_library(${ENGINE_NAME} STATIC 
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PRIVATE_SOURCES}
)

target_include_directories(${ENGINE_NAME} PUBLIC include/)
target_include_directories(${ENGINE_NAME} PRIVATE src/)

add_subdirectory(libs/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_NAME} PRIVATE glfw)

add_subdirectory(libs/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_NAME} PRIVATE spdlog)

add_subdirectory(libs/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_NAME} PRIVATE glad)

set(IMGUI_INCLUDES
	libs/imgui/imgui.h
	libs/imgui/backends/imgui_impl_glfw.h
	libs/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	libs/imgui/imgui.cpp
	libs/imgui/imgui_draw.cpp
	libs/imgui/imgui_demo.cpp
	libs/imgui/imgui_tables.cpp
	libs/imgui/imgui_widgets.cpp
	libs/imgui/backends/imgui_impl_glfw.cpp
	libs/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(ImGui STATIC 
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC libs)
target_include_directories(ImGui PUBLIC libs/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${ENGINE_NAME} PRIVATE ImGui)