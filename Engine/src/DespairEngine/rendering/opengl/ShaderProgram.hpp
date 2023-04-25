#ifndef ENGINE_SHADER_PROGRAM
#define ENGINE_SHADER_PROGRAM

#include <glm/mat4x4.hpp>

namespace engine
{
	class ShaderProgram
	{
	private:
		bool m_is_compiled = false;
		unsigned int m_id = 0;
	public:
		ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		void bind() const;
		static void unbind();
		bool isCompiled() const { return m_is_compiled; }
		void setMatrix4(const char* name, const glm::mat4& matrix) const;
		void setInt(const char* name, const int value) const;
		void setFloat(const char* name, const float value) const;
		void setVec3(const char* name, const glm::vec3& value) const;
	};
}

#endif // !ENGINE_SHADER_PROGRAM