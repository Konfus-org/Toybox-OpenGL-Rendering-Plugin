#pragma once
#include "IBindable.h"
#include <Tbx/Graphics/Texture.h>
#include <glad/glad.h>

namespace OpenGLRendering
{
    class OpenGLTexture : public IBindable
    {
    public:
        OpenGLTexture();
        ~OpenGLTexture() final;

        void Upload(const Tbx::Texture& tex, const Tbx::uint& slot);
        void Bind() const final;
        void Unbind() const final;

    private:
        Tbx::uint _slot = 0;
        Tbx::uint32 _rendererId = -1;
    };
}

