#ifndef ENGINE_VERTEX_BUFFER_HPP
#define ENGINE_VERTEX_BUFFER_HPP

namespace engine {

    class VertexBuffer {
    private:
        unsigned int m_id = 0;
    public:

        enum class EUsage
        {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void* data, const size_t size, const EUsage usage = VertexBuffer::EUsage::Static);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
        VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

        void bind() const;
        static void unbind();
    };

}

#endif // !ENGINE_VERTEX_BUFFER_HPP