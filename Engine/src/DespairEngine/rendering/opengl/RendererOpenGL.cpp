#include "RendererOpenGL.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "DespairEngine/Debugger.hpp"


namespace engine {

    bool RendererOpenGL::init(GLFWwindow* ptr_window)
    {
        glfwMakeContextCurrent(ptr_window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            DEBUG_CRITICAL("Failed to initialize GLAD");
            return false;
        }

        DEBUG_INFO("OpenGL context initialized:");
        DEBUG_INFO("  Vendor: {0}", getVendor_str());
        DEBUG_INFO("  Renderer: {0}", getRenderer_str());
        DEBUG_INFO("  Version: {0}", getVersion_str());

        return true;
    }

    void RendererOpenGL::draw(const VertexArray& vertex_array)
    {
        vertex_array.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.getIndicesCount()), GL_UNSIGNED_INT, nullptr);
    }

    void RendererOpenGL::setClearColor(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }

    void RendererOpenGL::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererOpenGL::setViewport(const unsigned int width, const unsigned int height, const unsigned int left_offset, const unsigned int bottom_offset)
    {
        glViewport(left_offset, bottom_offset, width, height);
    }

    void RendererOpenGL::enableDepthTest()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void RendererOpenGL::disableDepthTest()
    {
        glDisable(GL_DEPTH_TEST);
    }

    const char* RendererOpenGL::getVendor_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }

    const char* RendererOpenGL::getRenderer_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }

    const char* RendererOpenGL::getVersion_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

}