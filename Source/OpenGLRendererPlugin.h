#pragma once
#include <TbxCore.h>

namespace OpenGLRendering
{
    class OpenGLRendererFactory : public Tbx::IRendererFactoryPlugin
    {
    public:
        void OnLoad() override;
        void OnUnload() override;

        std::shared_ptr<Tbx::IRenderer> Create(std::shared_ptr<Tbx::IRenderSurface> surface) override;

    private:
        Tbx::IRenderer* New();
        void Delete(Tbx::IRenderer* renderer);
    };

    TBX_REGISTER_PLUGIN(OpenGLRendererFactory);
}