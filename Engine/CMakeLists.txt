cmake_minimum_required(VERSION 3.23)

set(ENGINE_NAME DespairEngine)

set(ENGINE_PUBLIC_INCLUDES
	include/DespairEngine/Core.hpp
	include/DespairEngine/Event.hpp
	include/DespairEngine/Debugger.hpp
	include/DespairEngine/Camera.hpp
	include/DespairEngine/Keys.hpp
	include/DespairEngine/Input.hpp
)

set(ENGINE_PRIVATE_INCLUDES
	src/DespairEngine/Window.hpp
	src/DespairEngine/modules/UIModule.hpp
	src/DespairEngine/rendering/opengl/ShaderProgram.hpp
	src/DespairEngine/rendering/opengl/VertexBuffer.hpp
	src/DespairEngine/rendering/opengl/VertexArray.hpp
	src/DespairEngine/rendering/opengl/IndexBuffer.hpp
	src/DespairEngine/rendering/opengl/RendererOpenGL.hpp
	src/DespairEngine/rendering/opengl/Texture2D.hpp
	src/DespairEngine/resources/loaders/ImageLoader.hpp
)

set(ENGINE_PRIVATE_SOURCES
	src/DespairEngine/Core.cpp
	src/DespairEngine/Window.cpp
	src/DespairEngine/Camera.cpp
	src/DespairEngine/Input.cpp
	src/DespairEngine/modules/UIModule.cpp
	src/DespairEngine/rendering/opengl/ShaderProgram.cpp
	src/DespairEngine/rendering/opengl/VertexBuffer.cpp
	src/DespairEngine/rendering/opengl/VertexArray.cpp
	src/DespairEngine/rendering/opengl/IndexBuffer.cpp
	src/DespairEngine/rendering/opengl/RendererOpenGL.cpp
	src/DespairEngine/rendering/opengl/Texture2D.cpp
	src/DespairEngine/resources/loaders/ImageLoader.cpp
)

set(ENGINE_ALL_SOURCES
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PRIVATE_SOURCES}
)
	

add_library(${ENGINE_NAME} STATIC 
	${ENGINE_ALL_SOURCES}
)

source_group(TREE  ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${ENGINE_ALL_SOURCES}
)

target_include_directories(${ENGINE_NAME} PUBLIC include)
target_include_directories(${ENGINE_NAME} PRIVATE src)
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_20)

add_subdirectory(libs/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_NAME} PRIVATE glfw)

add_subdirectory(libs/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_NAME} PRIVATE spdlog)

add_subdirectory(libs/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_NAME} PRIVATE glad)

add_subdirectory(libs/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_NAME} PRIVATE glm)

add_subdirectory(libs/soil ${CMAKE_CURRENT_BINARY_DIR}/soil)
target_link_libraries(${ENGINE_NAME} PRIVATE SOIL)

add_subdirectory(libs/freetype ${CMAKE_CURRENT_BINARY_DIR}/freetype)
target_link_libraries(${ENGINE_NAME} PRIVATE freetype)

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