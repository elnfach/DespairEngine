#ifndef ENGINE_SHADER_PROGRAM
#define ENGINE_SHADER_PROGRAM

namespace engine
{
	class ShaderProgram
	{
	private:
		bool m_isCompiled = false;
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
		bool isCompiled() const { return m_isCompiled; }
	};
}

#endif // !ENGINE_SHADER_PROGRAM