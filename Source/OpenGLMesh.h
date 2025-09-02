#pragma once
#include "IBindable.h"
#include "OpenGLBuffers.h"
#include <Tbx/Graphics/Buffers.h>
#include <Tbx/TypeAliases/Int.h>
#include <glad/glad.h>

namespace OpenGLRendering
{
    class OpenGLMesh : public IBindable
    {
    public:
        OpenGLMesh();
        ~OpenGLMesh() final;

        void Bind() const final;
        void Unbind() const final;

        void UploadVertexBuffer(const Tbx::VertexBuffer& buffer);
        void UploadIndexBuffer(const std::vector<Tbx::uint32>& buffer);

        const OpenGLVertexBuffer& GetVertexBuffer() const { return _vertexBuffer; }
        const OpenGLIndexBuffer& GetIndexBuffer() const { return _indexBuffer; }

    private:
        OpenGLVertexBuffer _vertexBuffer;
        OpenGLIndexBuffer _indexBuffer;
        Tbx::uint32 _rendererId = -1;
    };
}

