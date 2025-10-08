#pragma once
#include <Tbx/Graphics/GraphicsResource.h>
#include <Tbx/Graphics/Texture.h>

namespace Tbx::Plugins::OpenGLRendering
{
    class OpenGLTexture final : public TextureResource
    {
    public:
        OpenGLTexture(const Texture& tex);
        ~OpenGLTexture() override;

        void SetSlot(uint32 slot) override;

        void Activate() override;
        void Release() override;

    private:
        uint _slot = 0;
        uint32 _glId = -1;
    };
}

