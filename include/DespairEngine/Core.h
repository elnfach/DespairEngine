#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <DespairEngine/Debugger.h>

namespace engine
{
	struct WindowProperty
	{
		unsigned int width;
		unsigned int height;
		const char* title;
	};

	class Core
	{
		GLFWwindow* window;
		WindowProperty window_property;
	public:

		Core();
		virtual ~Core();

		Core(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) = delete;

		int start(unsigned int width, unsigned int height, const char* title);
		virtual void update() = 0;
	};
}