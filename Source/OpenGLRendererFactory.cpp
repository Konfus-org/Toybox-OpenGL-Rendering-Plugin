#include "OpenGLRendererFactory.h"
#include "OpenGLRenderer.h"

namespace OpenGLRendering
{
    void OpenGLRendererFactory::OnLoad()
    {
        // Do nothing...
    }

    void OpenGLRendererFactory::OnUnload()
    {
        // Do nothing...
    }

    std::shared_ptr<Tbx::IRenderer> OpenGLRendererFactory::Create(std::shared_ptr<Tbx::IRenderSurface> surface)
    {
        auto renderer = std::shared_ptr<Tbx::IRenderer>(New(), [this](Tbx::IRenderer* renderer) { Delete(renderer); });
        renderer->Initialize(surface);
        return renderer;
    }

    Tbx::IRenderer* OpenGLRendererFactory::New()
    {
        auto* renderer = new OpenGLRenderer();
        return renderer;
    }

    void OpenGLRendererFactory::Delete(Tbx::IRenderer* renderer)
    {
        delete renderer;
    }
}