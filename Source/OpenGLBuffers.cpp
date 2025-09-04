#include "OpenGLBuffers.h"
#include "OpenGLMaterial.h"

namespace OpenGLRendering
{
    /// Vertex Buffer ///////////////////////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer()
    {
        glCreateBuffers(1, &_vertBufferGLId);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        Unbind();
        glDeleteBuffers(1, &_vertBufferGLId);
    }

    void OpenGLVertexBuffer::Upload(const Tbx::VertexBuffer& vertices)
    {
        const auto& verticesVec = vertices.GetVertices();
        _count = (Tbx::uint32)verticesVec.size();
        glBufferData(GL_ARRAY_BUFFER, _count * sizeof(float), verticesVec.data(), GL_STATIC_DRAW);

        Tbx::uint32 index = 0;
        const auto& layout = vertices.GetLayout();
        for (const auto& element : layout)
        {
            const auto& count = element.GetCount();
            const auto& type = ShaderUniformTypeToOpenGLType(element.GetType());
            const auto& stride = layout.GetStride();
            const auto& offset = element.GetOffset();
            const auto& normalized = element.IsNormalized() ? GL_TRUE : GL_FALSE;

            AddAttribute(index, count, type, stride, offset, normalized);

            index++;
        }
    }

    void OpenGLVertexBuffer::AddAttribute(const Tbx::uint& index, const Tbx::uint& size, const Tbx::uint& type, const Tbx::uint& stride, const Tbx::uint& offset, const bool& normalized) const
    {
        // Need to do this casting nonsense to get rid of a warning...
        const auto* offsetPtr = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(offset));
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, offsetPtr);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vertBufferGLId);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /// Index Buffer ////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer()
    {
        glCreateBuffers(1, &_indexBuffGLId);
    }

    void OpenGLIndexBuffer::Upload(const std::vector<Tbx::uint32>& indices)
    {
        _count = (Tbx::uint32)indices.size();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(Tbx::uint32), indices.data(), GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        Unbind();
        glDeleteBuffers(1, &_indexBuffGLId);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffGLId);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
