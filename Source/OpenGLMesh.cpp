#include "OpenGLMesh.h"
#include "OpenGLBuffers.h"
#include "OpenGLMaterial.h"

namespace OpenGLRendering
{
    OpenGLMesh::OpenGLMesh()
    {
        glGenVertexArrays(1, &_rendererId);
    }

    OpenGLMesh::~OpenGLMesh()
    {
        glDeleteVertexArrays(1, &_rendererId);
    }

    void OpenGLMesh::UploadVertexBuffer(const Tbx::VertexBuffer& buffer)
    {
        TBX_ASSERT(buffer.GetLayout().GetElements().size(), "Vertex buffer has no layout... a layout MUST be provided!");
        _vertexBuffer.Bind();
        _vertexBuffer.Upload(buffer);
    }

    void OpenGLMesh::UploadIndexBuffer(const std::vector<Tbx::uint32>& buffer)
    {
        _indexBuffer.Bind();
        _indexBuffer.Upload(buffer);
    }

    void OpenGLMesh::Bind() const
    {
        _vertexBuffer.Bind();
        _indexBuffer.Bind();
        glBindVertexArray(_rendererId);
    }

    void OpenGLMesh::Unbind() const
    {
        _vertexBuffer.Unbind();
        _indexBuffer.Unbind();
        glBindVertexArray(0);
    }
}