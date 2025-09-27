#pragma once
#include "IBindable.h"
#include <Tbx/Graphics/Mesh.h>
#include <Tbx/Math/Int.h>

namespace OpenGLRendering
{
    class OpenGLVertexBuffer : public IBindable
    {
    public:
        OpenGLVertexBuffer();
        ~OpenGLVertexBuffer() final;

        void Upload(const Tbx::VertexBuffer& buffer);
        void AddAttribute(Tbx::uint index, Tbx::uint32 size, Tbx::uint32 type, Tbx::uint32 stride, Tbx::uint32 offset, bool normalized) const;

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

        void Upload(const Tbx::IndexBuffer& buffer);
        void Bind() const final;
        void Unbind() const final;

        Tbx::uint32 GetCount() const { return _count; }

    private:
        Tbx::uint32 _indexBuffGLId = -1;
        Tbx::uint32 _count = 0;
    };
}
