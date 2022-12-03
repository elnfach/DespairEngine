#include <glfw/glfw3.h>

#include <imgui/imgui.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

#include "Window.hpp"
#include "DespairEngine/Input.hpp"
#include "DespairEngine/Camera.hpp"
#include "DespairEngine/Debugger.hpp"


#include "DespairEngine/modules/UIModule.hpp"
#include "DespairEngine/rendering/opengl/ShaderProgram.hpp"
#include "DespairEngine/rendering/opengl/VertexBuffer.hpp"
#include "DespairEngine/rendering/opengl/VertexArray.hpp"
#include "DespairEngine/rendering/opengl/IndexBuffer.hpp"
#include "DespairEngine/rendering/opengl/RendererOpenGL.hpp"

#include "DespairEngine/Core.hpp"

namespace engine
{
	GLfloat positions_colors2[] = {
		0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
		0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
		0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1
	};

	const char* vertex_shader =
		R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_color;
           uniform mat4 model_matrix;
           uniform mat4 view_projection_matrix;
           out vec3 color;
           void main() {
              color = vertex_color;
              gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
           }
        )";

	const char* fragment_shader =
		R"(#version 460
           in vec3 color;
           out vec4 frag_color;
           void main() {
              frag_color = vec4(color, 1.0);
           }
        )";

	std::unique_ptr<ShaderProgram> p_shader_program;
	std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
	std::unique_ptr<IndexBuffer> p_index_buffer;
	std::unique_ptr<VertexArray> p_vao;
	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };
	float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };

	Core::Core()
	{
		DEBUG_INFO("Starting Application");
	}
	
	Core::~Core()
	{
		DEBUG_INFO("Closing Application");
	}

	int Core::start(const char* title, unsigned int width, unsigned int height)
	{
		m_ptr_window = std::make_unique<Window>(title, width, height);
		
		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				// DEBUG_INFO("[Mouse Moved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);
		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				DEBUG_INFO("[RESIZED] Changed size to {0}x{1}", event.width, event.height);
			}
		);
		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event)
			{
				DEBUG_INFO("[Window Closed]");
				m_isClosed = true;
			}
		);

		m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
			[&](EventMouseButtonPressed& event)
			{
				DEBUG_INFO("[Mouse button pressed: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
				Input::PressMouseButton(event.mouse_button);
				onMouseButtonEvent(event.mouse_button, event.x_pos, event.y_pos, true);
			});

		m_event_dispatcher.add_event_listener<EventMouseButtonReleased>(
			[&](EventMouseButtonReleased& event)
			{
				DEBUG_INFO("[Mouse button released: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
				Input::ReleaseMouseButton(event.mouse_button);
				onMouseButtonEvent(event.mouse_button, event.x_pos, event.y_pos, false);
			});

		m_event_dispatcher.add_event_listener<EventKeyPressed>(
			[&](EventKeyPressed& event)
			{
				if (event.key_code <= KeyCode::KEY_Z)
				{
					if (event.repeated)
					{
						DEBUG_INFO("[Key pressed: {0}, repeated", static_cast<char>(event.key_code));
					}
					else
					{
						DEBUG_INFO("[Key pressed: {0}", static_cast<char>(event.key_code));
					}
				}
				Input::PressKey(event.key_code);
			});

		m_event_dispatcher.add_event_listener<EventKeyReleased>(
			[&](EventKeyReleased& event)
			{
				if (event.key_code <= KeyCode::KEY_Z)
				{
					DEBUG_INFO("[Key released: {0}", static_cast<char>(event.key_code));
				}
				Input::ReleaseKey(event.key_code);
			});

		m_ptr_window->set_event_callback(
			[&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);

		//---------------------------------------//
		p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!p_shader_program->isCompiled())
		{
			return false;
		}

		BufferLayout buffer_layout_1vec3
		{
			ShaderDataType::Float3
		};


		BufferLayout buffer_layout_2vec3
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};

		p_vao = std::make_unique<VertexArray>();
		p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors2, sizeof(positions_colors2), buffer_layout_2vec3);
		p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		p_vao->add_vertex_buffer(*p_positions_colors_vbo);
		p_vao->set_index_buffer(*p_index_buffer);
		//---------------------------------------//

		while (!m_isClosed)
		{
			RendererOpenGL::setClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
			RendererOpenGL::clear();

			p_shader_program->bind();

			glm::mat4 scale_matrix(scale[0], 0, 0, 0,
				0, scale[1], 0, 0,
				0, 0, scale[2], 0,
				0, 0, 0, 1);

			float rotate_in_radians = glm::radians(rotate);
			glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
				-sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

			glm::mat4 translate_matrix(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate[0], translate[1], translate[2], 1);

			glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
			p_shader_program->setMatrix4("model_matrix", model_matrix);

			camera.setProjectionMode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
			p_shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
			RendererOpenGL::draw(*p_vao);


			//---------------------------------------//
			UIModule::onUIDrawBegin();
			bool show = true;
			UIModule::showExampleAppDockSpace(&show);
			ImGui::ShowDemoWindow();
			ImGui::Begin("Background Color Window");
			ImGui::ColorEdit4("Background Color", m_background_color);
			ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
			ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
			ImGui::SliderFloat3("translate", translate, -1.f, 1.f);
			ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
			ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
			ImGui::Checkbox("Perspective camera", &perspective_camera);
			ImGui::End();
			//---------------------------------------//

			onUIDraw();

			UIModule::onUIDrawEnd();

			m_ptr_window->update();
			update();
		}
		m_ptr_window = nullptr;

		return 0;
	}

	glm::vec2 Core::getCurrentCursorPosition() const
	{
		return m_ptr_window->getCurrentCursorPosition();
	}
}