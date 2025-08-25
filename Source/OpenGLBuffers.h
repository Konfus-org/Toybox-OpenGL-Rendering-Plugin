#pragma once
#include "IBindable.h"
#include <Tbx/Graphics/Buffers.h>
#include <Tbx/TypeAliases/Int.h>
#include <glad/glad.h>

namespace OpenGLRendering
{
    class OpenGLVertexBuffer : public IBindable
    {
    public:
        OpenGLVertexBuffer();
        ~OpenGLVertexBuffer() final;

        void Upload(const Tbx::VertexBuffer& vertices);
        void AddAttribute(const Tbx::uint& index, const Tbx::uint& size, const Tbx::uint& type, const Tbx::uint& stride, const Tbx::uint& offset, const bool& normalized) const;

        void Bind() const final;
        void Unbind() const final;

        Tbx::uint32 GetCount() const { return _count; }

    private:
        Tbx::uint32 _vertBufferGLId = -1;
        Tbx::uint32 _count = 0;
    };

    class OpenGLIndexBuffer : public IBindable
    {
    public:
        OpenGLIndexBuffer();
        ~OpenGLIndexBuffer() final;

        void Upload(const std::vector<Tbx::uint32>& indices);
        void Bind() const final;
        void Unbind() const final;

        Tbx::uint32 GetCount() const { return _count; }

    private:
        Tbx::uint32 _indexBuffGLId = -1;
        Tbx::uint32 _count = 0;
    };

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
