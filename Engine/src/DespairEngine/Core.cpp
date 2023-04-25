#include <glfw/glfw3.h>

#include <imgui/imgui.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

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
#include "DespairEngine/rendering/opengl/Texture2D.hpp"

#include "DespairEngine/Core.hpp"

namespace engine
{
	GLfloat pos_norm_uv[] = {
		//    position             normal            UV                  index

		// FRONT
		-1.0f, -1.f, -1.f,    -1.f,  0.f,  0.f,     0.f, 0.f,              // 0
		-1.0f,  1.f, -1.f,    -1.f,  0.f,  0.f,     1.f, 0.f,              // 1
		-1.0f,  1.f,  1.f,    -1.f,  0.f,  0.f,     1.f, 1.f,              // 2
		-1.0f, -1.f,  1.f,    -1.f,  0.f,  0.f,     0.f, 1.f,              // 3

		// BACK                                  
		 1.0f, -1.f, -1.f,     1.f,  0.f,  0.f,     1.f, 0.f,              // 4
		 1.0f,  1.f, -1.f,     1.f,  0.f,  0.f,     0.f, 0.f,              // 5
		 1.0f,  1.f,  1.f,     1.f,  0.f,  0.f,     0.f, 1.f,              // 6
		 1.0f, -1.f,  1.f,     1.f,  0.f,  0.f,     1.f, 1.f,              // 7

		 // RIGHT
		 -1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     0.f, 0.f,              // 8
		  1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     1.f, 0.f,              // 9
		  1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     1.f, 1.f,              // 10
		 -1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     0.f, 1.f,              // 11

		 // LEFT
		 -1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     1.f, 0.f,              // 12
		  1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     0.f, 0.f,              // 13
		  1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     0.f, 1.f,              // 14
		 -1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     1.f, 1.f,              // 15

		 // TOP
		 -1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 0.f,              // 16
		 -1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 0.f,              // 17
		  1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 1.f,              // 18
		  1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 1.f,              // 19

		  // BOTTOM
		  -1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 1.f,              // 20
		  -1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 1.f,              // 21
		   1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 0.f,              // 22
		   1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 0.f,              // 23
	};


	GLuint indices[] = {
		0,   1,  2,  2,  3,  0, // front
		4,   5,  6,  6,  7,  4, // back
		8,   9, 10, 10, 11,  8, // right
		12, 13, 14, 14, 15, 12, // left
		16, 17, 18, 18, 19, 16, // top
		20, 21, 22, 22, 23, 20  // bottom
	};

	void generate_circle(unsigned char* data,
		const unsigned int width,
		const unsigned int height,
		const unsigned int center_x,
		const unsigned int center_y,
		const unsigned int radius,
		const unsigned char color_r,
		const unsigned char color_g,
		const unsigned char color_b)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius)
				{
					data[3 * (x + width * y) + 0] = color_r;
					data[3 * (x + width * y) + 1] = color_g;
					data[3 * (x + width * y) + 2] = color_b;
				}
			}
		}
	}

	void generate_smile_texture(unsigned char* data,
		const unsigned int width,
		const unsigned int height)
	{
		// background
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				data[3 * (x + width * y) + 0] = 200;
				data[3 * (x + width * y) + 1] = 191;
				data[3 * (x + width * y) + 2] = 231;
			}
		}

		// face
		generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);

		// smile
		generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
		generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);

		// eyes
		generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
		generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
	}

	void generate_quads_texture(unsigned char* data,
		const unsigned int width,
		const unsigned int height)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if ((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2)
				{
					data[3 * (x + width * y) + 0] = 0;
					data[3 * (x + width * y) + 1] = 0;
					data[3 * (x + width * y) + 2] = 0;
				}
				else
				{
					data[3 * (x + width * y) + 0] = 255;
					data[3 * (x + width * y) + 1] = 255;
					data[3 * (x + width * y) + 2] = 255;
				}
			}
		}
	}

	const char* vertex_shader =
		R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_normal;
           layout(location = 2) in vec2 texture_coord;

           uniform mat4 model_matrix;
           uniform mat4 view_projection_matrix;
		   uniform int current_frame; 

		   out vec2 tex_coord_smile;
           out vec2 tex_coord_quads;
		   out vec3 frag_position;
           out vec3 frag_normal;
           void main() {

			  tex_coord_smile = texture_coord;
              tex_coord_quads = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);
              frag_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
              vec4 vertex_position_world = model_matrix * vec4(vertex_position, 1.0);
              frag_position = vertex_position_world.xyz;
              gl_Position = view_projection_matrix * vertex_position_world;
           }
        )";

	const char* fragment_shader =
		R"(#version 460

		   in vec2 tex_coord_smile;
           in vec2 tex_coord_quads;
		   in vec3 frag_position;
           in vec3 frag_normal;

           layout (binding = 0) uniform sampler2D InTexture_Smile;
           layout (binding = 1) uniform sampler2D InTexture_Quads;

		   uniform vec3 camera_position;
           uniform vec3 light_position;
           uniform vec3 light_color;
           uniform float ambient_factor;
           uniform float diffuse_factor;
           uniform float specular_factor;
           uniform float shininess;

           out vec4 frag_color;
           void main() {
              // ambient
              vec3 ambient = ambient_factor * light_color;
              // diffuse
              vec3 normal = normalize(frag_normal);
              vec3 light_dir = normalize(light_position - frag_position);
              vec3 diffuse = diffuse_factor * light_color * max(dot(normal, light_dir), 0.0);
              // specular
              vec3 view_dir = normalize(camera_position - frag_position);
              vec3 reflect_dir = reflect(-light_dir, normal);
              float specular_value = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
              vec3 specular = specular_factor * specular_value * light_color;
              //frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
              frag_color = texture(InTexture_Smile, tex_coord_smile) * vec4(ambient + diffuse + specular, 1.f);
           }
    )";

	const char* light_source_vertex_shader =
		R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_normal;
           layout(location = 2) in vec2 texture_coord;
           uniform mat4 model_matrix;
           uniform mat4 view_projection_matrix;
           void main() {
              gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position * 0.1f, 1.0);
           }
        )";

	const char* light_source_fragment_shader =
		R"(#version 460
           out vec4 frag_color;
           uniform vec3 light_color;
           void main() {
              frag_color = vec4(light_color, 1.f);
           }
        )";

	std::unique_ptr<ShaderProgram> p_shader_program;
	std::unique_ptr<ShaderProgram> p_light_source_shader_program;
	std::unique_ptr<VertexBuffer> p_cube_positions_vbo;
	std::unique_ptr<IndexBuffer> p_cube_index_buffer;
	std::unique_ptr<VertexArray> p_vao;
	std::unique_ptr<Texture2D> p_texture_smile;
	std::unique_ptr<Texture2D> p_texture_quads;
	std::unique_ptr<VertexArray> p_cube_vao;
	float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };

	std::array<glm::vec3, 5> positions = {
			glm::vec3(-2.f, -2.f, -4.f),
			glm::vec3(-5.f,  0.f,  3.f),
			glm::vec3(2.f,  1.f, -2.f),
			glm::vec3(4.f, -3.f,  3.f),
			glm::vec3(1.f, -7.f,  1.f)
	};

	Core::Core()
	{
		DEBUG_INFO("Starting Application");
	}
	
	Core::~Core()
	{
		DEBUG_INFO("Closing Application");
	}

	void Core::draw()
	{
		RendererOpenGL::setClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
		RendererOpenGL::clear();

		p_shader_program->bind();

		//glm::mat4 scale_matrix(scale[0], 0, 0, 0,
        //    0, scale[1], 0, 0,
        //    0, 0, scale[2], 0,
        //    0, 0, 0, 1);

        //float rotate_in_radians = glm::radians(rotate);
        //glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
        //    -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
        //    0, 0, 1, 0,
        //    0, 0, 0, 1);

		//glm::mat4 translate_matrix(1, 0, 0, 0,
		//    0, 1, 0, 0,
		//    0, 0, 1, 0,
		//    translate[0], translate[1], translate[2], 1);

		//glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
		//p_shader_program->setMatrix4("model_matrix", model_matrix);
		static int current_frame = 0;
		p_shader_program->setInt("current_frame", current_frame++);

		p_shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());

		p_shader_program->setVec3("camera_position", camera.getPosition());
		p_shader_program->setVec3("light_position", glm::vec3(light_source_position[0], light_source_position[1], light_source_position[2]));
		p_shader_program->setVec3("light_color", glm::vec3(light_source_color[0], light_source_color[1], light_source_color[2]));
		p_shader_program->setFloat("ambient_factor", ambient_factor);
		p_shader_program->setFloat("diffuse_factor", diffuse_factor);
		p_shader_program->setFloat("specular_factor", specular_factor);
		p_shader_program->setFloat("shininess", shininess);

		// cubes

		for (const glm::vec3& current_position : positions)
		{
			glm::mat4 translate_matrix(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				current_position[0], current_position[1], current_position[2], 1);
			p_shader_program->setMatrix4("model_matrix", translate_matrix);
			RendererOpenGL::draw(*p_cube_vao);
		}
		// light source
		{
			p_light_source_shader_program->bind();
			p_light_source_shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
			glm::mat4 translate_matrix(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				light_source_position[0], light_source_position[1], light_source_position[2], 1);
			p_light_source_shader_program->setMatrix4("model_matrix", translate_matrix);
			p_light_source_shader_program->setVec3("light_color", glm::vec3(light_source_color[0], light_source_color[1], light_source_color[2]));
			RendererOpenGL::draw(*p_cube_vao);
		}

		UIModule::onUIDrawBegin();
		onUIDraw();
		UIModule::onUIDrawEnd();

		m_ptr_window->update();
		update();
	}


	int Core::start(const char* title, unsigned int width, unsigned int height)
	{
		m_ptr_window = std::make_unique<Window>(title, width, height);
		camera.setViewportSize(static_cast<float>(width), static_cast<float>(height));
		
		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				// DEBUG_INFO("[Mouse Moved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);
		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[&](EventWindowResize& event)
			{
				DEBUG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
				camera.setViewportSize(event.width, event.height);
				draw();
			}
		);
		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event)
			{
				DEBUG_INFO("[Window Closed]");
				close();
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

		const unsigned int n_width = 1000;
		const unsigned int n_height = 1000;
		const unsigned int n_channels = 3;
		auto* data = new unsigned char[n_width * n_height * n_channels];

		generate_smile_texture(data, n_width, n_height);
		p_texture_smile = std::make_unique<Texture2D>(data, n_width, n_height);
		p_texture_smile->bind(0);

		generate_quads_texture(data, n_width, n_height);
		p_texture_quads = std::make_unique<Texture2D>(data, n_width, n_height);
		p_texture_quads->bind(1);

		delete[] data;

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


		BufferLayout buffer_layout_vec3_vec3_vec2
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3,
			ShaderDataType::Float2
		};

		p_cube_vao = std::make_unique<VertexArray>();
		p_cube_positions_vbo = std::make_unique<VertexBuffer>(pos_norm_uv, sizeof(pos_norm_uv), buffer_layout_vec3_vec3_vec2);
		p_cube_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		p_cube_vao->add_vertex_buffer(*p_cube_positions_vbo);
		p_cube_vao->set_index_buffer(*p_cube_index_buffer);

		//---------------------------------------//

		p_light_source_shader_program = std::make_unique<ShaderProgram>(light_source_vertex_shader, light_source_fragment_shader);
		if (!p_light_source_shader_program->isCompiled())
		{
			return false;
		}

		RendererOpenGL::enableDepthTest();
		while (!m_window_is_closed)
		{
			draw();
		}

		m_ptr_window = nullptr;

		return 0;
	}

	glm::vec2 Core::getCurrentCursorPosition() const
	{
		return m_ptr_window->getCurrentCursorPosition();
	}

	void Core::close()
	{
		m_window_is_closed = true;
	}
}