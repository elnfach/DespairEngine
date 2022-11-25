#ifndef ENGINE_INDEX_BUFFER_HPP
#define ENGINE_INDEX_BUFFER_HPP

#include <glad/glad.h>
#include <DespairEngine/Debugger.hpp>

#include <DespairEngine/rendering/opengl/VertexBuffer.hpp>

namespace engine {

    class IndexBuffer {
    public:

        IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer& operator=(IndexBuffer&& index_buffer) noexcept;
        IndexBuffer(IndexBuffer&& index_buffer) noexcept;

        void bind() const;
        static void unbind();
        size_t get_count() const { return m_count; }

    private:
        unsigned int m_id = 0;
        size_t m_count;
    };

}

#endif // !ENGINE_INDEX_BUFFER_HPP
