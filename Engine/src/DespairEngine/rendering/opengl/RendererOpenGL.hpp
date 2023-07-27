#ifndef ENGINE_RENDER_OPENGL_HPP
#define ENGINE_RENDER_OPENGL_HPP

struct GLFWwindow;

namespace engine {
    class VertexArray;

    class RendererOpenGL {
    public:
        static bool init(GLFWwindow* ptr_window);

        static void draw(const VertexArray& vertex_array);
        static void initText();
        //static void drawText(const glm::vec2& pos, float scale, const char* text);
        static void setClearColor(const float r, const float g, const float b, const float a);
        static void clear();
        static void setViewport(const unsigned int width, const unsigned int height, const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);

        static void enableDepthTest();
        static void disableDepthTest();

        static const char* getVendor_str();
        static const char* getRenderer_str();
        static const char* getVersion_str();
    };
}

#endif // !ENGINE_RENDER_OPENGL_HPP