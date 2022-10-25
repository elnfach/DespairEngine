cmake_minimum_required(VERSION 3.23)

set(EDITOR_NAME DespairEngineEditor)

add_executable(${EDITOR_NAME}
	main.cpp
)

target_link_libraries(${EDITOR_NAME} DespairEngine glfw glad spdlog ImGui)
set_target_properties(${EDITOR_NAME} PROPERTIES RUNTIME_OUTPUT_DERICTORIES ${CMAKE_BINARY_DIR}/bin/)