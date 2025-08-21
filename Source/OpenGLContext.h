#pragma once
#include <Tbx/Graphics/IRenderSurface.h>
#include <memory>

namespace OpenGLRendering
{
    class OpenGLContext
    {
    public:
        OpenGLContext() = default;
        ~OpenGLContext() = default;

        void Set(const std::weak_ptr<Tbx::IRenderSurface>& surface);
        void SwapBuffers();

        int GetSwapInterval() const;
        void SetSwapInterval(int interval) const;

        std::weak_ptr<Tbx::IRenderSurface> GetRenderSurface();

    private:
        std::weak_ptr<Tbx::IRenderSurface> _renderSurface = {};
    };
}

