#include <vector>

#include <glad/glad.h>

#include "DespairEngine/Debugger.hpp"

#include "VertexBuffer.hpp"

namespace engine {

    constexpr unsigned int shaderDataTypeToComponentsCount(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Int:
            return 1;

        case ShaderDataType::Float2:
        case ShaderDataType::Int2:
            return 2;

        case ShaderDataType::Float3:
        case ShaderDataType::Int3:
            return 3;

        case ShaderDataType::Float4:
        case ShaderDataType::Int4:
            return 4;
        }

        DEBUG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
        return 0;
    }


    constexpr size_t shaderDataTypeSize(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return sizeof(GLfloat) * shaderDataTypeToComponentsCount(type);

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return sizeof(GLint) * shaderDataTypeToComponentsCount(type);
        }

        DEBUG_ERROR("shader_data_type_size: unknown ShaderDataType!");
        return 0;
    }


    constexpr unsigned int shaderDataTypeToComponentType(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return GL_FLOAT;

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        }

        DEBUG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
        return GL_FLOAT;
    }

    constexpr GLenum usageToGLenum(const VertexBuffer::EUsage usage)
    {
        switch (usage)
        {
        case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
        case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
        case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
        }

        DEBUG_ERROR("Unknown VertexBuffer usage");
        return GL_STREAM_DRAW;
    }

    BufferElement::BufferElement(const ShaderDataType _type)
        : type(_type)
        , component_type(shaderDataTypeToComponentType(_type))
        , components_count(shaderDataTypeToComponentType(_type))
        , size(shaderDataTypeSize(_type))
        , offset(0)
    {
    }


    VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, const EUsage usage)
        : m_buffer_layout(std::move(buffer_layout))
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usageToGLenum(usage));
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept
    {
        m_id = vertexBuffer.m_id;
        vertexBuffer.m_id = 0;
        return *this;
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
        : m_id(vertexBuffer.m_id)
        , m_buffer_layout(std::move(vertexBuffer.m_buffer_layout))
    {
        vertexBuffer.m_id = 0;
    }
}