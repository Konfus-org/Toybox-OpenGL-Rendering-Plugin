#pragma once
#include <Tbx/Plugins/Plugin.h>
#include <Tbx/Graphics/IRenderer.h>

namespace OpenGLRendering
{
    class OpenGLRendererFactory : public Tbx::IRendererFactory, public Tbx::Plugin
    {
    public:
        OpenGLRendererFactory(const std::weak_ptr<Tbx::App>& app) {}
        std::shared_ptr<Tbx::IRenderer> Create(std::shared_ptr<Tbx::IRenderSurface> surface) override;

    private:
        Tbx::IRenderer* New();
        void Delete(Tbx::IRenderer* renderer);
    };

    TBX_REGISTER_PLUGIN(OpenGLRendererFactory);
}