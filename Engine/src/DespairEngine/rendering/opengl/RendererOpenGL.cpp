#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gl/GL.h>

#include <glm/mat3x3.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "VertexArray.hpp"
#include "DespairEngine/Debugger.hpp"

#include "RendererOpenGL.hpp"

namespace engine {

    const char* glSourceToString(const GLenum source)
    {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "DEBUG_SOURCE_API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG_SOURCE_WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG_SOURCE_THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "DEBUG_SOURCE_APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "DEBUG_SOURCE_OTHER";

        default: return "UNKNOWN_DEBUG_SOURCE";
        }
    }

    const char* glTypeToString(const GLenum type)
    {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "DEBUG_TYPE_ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "DEBUG_TYPE_PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG_TYPE_PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "DEBUG_TYPE_MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG_TYPE_PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG_TYPE_POP_GROUP";
        case GL_DEBUG_TYPE_OTHER: return "DEBUG_TYPE_OTHER";

        default: return "UNKNOWN_DEBUG_TYPE";
        }
    }

    bool RendererOpenGL::init(GLFWwindow* ptr_window)
    {
        glfwMakeContextCurrent(ptr_window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            DEBUG_CRITICAL("Failed to initialize GLAD");
            return false;
        }

        DEBUG_INFO("OpenGL context initialized:");
        DEBUG_INFO("  OpenGL Vendor: {0}", getVendor_str());
        DEBUG_INFO("  OpenGL Renderer: {0}", getRenderer_str());
        DEBUG_INFO("  OpenGL Version: {0}", getVersion_str());

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        glDebugMessageCallback([](GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
            {
                switch (severity)
                {
                case GL_DEBUG_SEVERITY_HIGH:
                    DEBUG_INFO("OpenGL Error: [{0}:{1}]({2}): {3}",
                        glSourceToString(source),
                        glTypeToString(type),
                        id,
                        message);
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    DEBUG_WARNING("OpenGL Warning: [{0}:{1}]({2}): {3}",
                        glSourceToString(source),
                        glTypeToString(type),
                        id,
                        message);
                    break;
                case GL_DEBUG_SEVERITY_LOW:
                    DEBUG_INFO("OpenGL Info: [{0}:{1}]({2}): {3}",
                        glSourceToString(source),
                        glTypeToString(type),
                        id,
                        message);
                    break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    DEBUG_INFO("OpenGL Notificaton: [{0}:{1}]({2}): {3}",
                        glSourceToString(source),
                        glTypeToString(type),
                        id,
                        message);
                    break;
                default:
                    DEBUG_ERROR("OpenGL Error: [{0}:{1}] ({2}) : {3}",
                        glSourceToString(source),
                        glTypeToString(type),
                        id,
                        message);
                }
            }, nullptr);

        return true;
    }

    void RendererOpenGL::draw(const VertexArray& vertex_array)
    {
        vertex_array.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.getIndicesCount()), GL_UNSIGNED_INT, nullptr);
    }

    void RendererOpenGL::initText()
    {
    }

    /*void RendererOpenGL::drawText(const glm::vec2& pos, float scale, const char* text)
    {
    }*/

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