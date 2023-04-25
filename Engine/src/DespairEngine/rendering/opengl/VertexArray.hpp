#ifndef ENGINE_VERTEX_ARRAY_HPP
#define EGNINE_VERTEX_ARRAY_HPP

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace engine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertex_buffer) noexcept;
        VertexArray(VertexArray&& vertex_buffer) noexcept;

        void addVertexBuffer(const VertexBuffer& vertex_buffer);
        void setIndexBuffer(const IndexBuffer& index_buffer);
        void bind() const;
        static void unbind();
        size_t getIndicesCount() const { return m_indices_count; }

    private:
        unsigned int m_id = 0;
        unsigned int m_elements_count = 0;
        size_t m_indices_count = 0;
    };

}

#endif // !ENGINE_VERTEX_ARRAY
