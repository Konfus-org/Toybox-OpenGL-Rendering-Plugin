#pragma once
#include <Tbx/Plugins/Plugin.h>
#include <Tbx/Graphics/IRenderer.h>

namespace OpenGLRendering
{
    class OpenGLRendererFactoryPlugin
        : public Tbx::Plugin
        , public Tbx::IRendererFactory
    {
    public:
        OpenGLRendererFactoryPlugin(const Tbx::WeakRef<Tbx::App>& app) {}
        std::shared_ptr<Tbx::IRenderer> Create() override;

    private:
        void InitializeOpenGl();
        Tbx::IRenderer* New();
        void Delete(Tbx::IRenderer* renderer);

    private:
        bool _isGlInitialized = false;
    };

    TBX_REGISTER_PLUGIN(OpenGLRendererFactoryPlugin);
}