#include "OpenGLMesh.h"
#include "OpenGLBuffers.h"
#include <glad/glad.h>

namespace Tbx::Plugins::OpenGLRendering
{
    OpenGLRendering::OpenGLMesh::OpenGLMesh(const Mesh& mesh)
    {
        auto id = static_cast<uint32>(RenderId);
        glGenVertexArrays(1, &id);

        SetVertexBuffer(mesh.Vertices);
        SetIndexBuffer(mesh.Indices);
    }

    OpenGLMesh::~OpenGLMesh()
    {
        auto id = static_cast<uint32>(RenderId);
        glDeleteVertexArrays(1, &id);
    }

    void OpenGLMesh::SetVertexBuffer(const VertexBuffer& buffer)
    {
        TBX_ASSERT(buffer.Layout.Elements.size(), "GL Rendering: Vertex buffer has no layout... a layout MUST be provided!");
        _vertexBuffer.Bind();
        _vertexBuffer.Upload(buffer);
    }

    void OpenGLMesh::SetIndexBuffer(const IndexBuffer& buffer)
    {
        _indexBuffer.Bind();
        _indexBuffer.Upload(buffer);
    }

    void OpenGLMesh::Activate()
    {
        _vertexBuffer.Bind();
        _indexBuffer.Bind();
        glBindVertexArray(RenderId);
    }

    void OpenGLMesh::Release()
    {
        _vertexBuffer.Unbind();
        _indexBuffer.Unbind();
        glBindVertexArray(0);
    }
}