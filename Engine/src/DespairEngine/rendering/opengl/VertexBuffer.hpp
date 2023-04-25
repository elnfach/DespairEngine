#ifndef ENGINE_VERTEX_BUFFER_HPP
#define ENGINE_VERTEX_BUFFER_HPP

#include <vector>

namespace engine {

    enum class ShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
    };

    struct BufferElement
    {
        ShaderDataType type;
        uint32_t component_type;
        size_t components_count;
        size_t size;
        size_t offset;

        BufferElement(const ShaderDataType type);
    };

    class BufferLayout
    {
    public:
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements(std::move(elements))
        {
            size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements)
            {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

        const std::vector<BufferElement>& getElements() const { return m_elements; }
        size_t getStride() const { return m_stride; }

    private:
        std::vector<BufferElement> m_elements;
        size_t m_stride = 0;
    };


    class VertexBuffer {
    private:
        unsigned int m_id = 0;
        BufferLayout m_buffer_layout;
    public:

        enum class EUsage
        {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, const EUsage usage = VertexBuffer::EUsage::Static);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
        VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

        unsigned int getHandle() const { return m_id; }

        const BufferLayout& getLayout() const { return m_buffer_layout; }
    };

}

#endif // !ENGINE_VERTEX_BUFFER_HPP