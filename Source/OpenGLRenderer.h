#pragma once
#include "OpenGLContext.h"
#include "OpenGLBuffers.h"
#include "OpenGLMaterial.h"
#include <glad/glad.h>
#include <Tbx/Graphics/IRenderer.h>
#include <Tbx/Graphics/Texture.h>
#include <Tbx/Graphics/Mesh.h>
#include <Tbx/Graphics/Material.h>

namespace OpenGLRendering
{
    class OpenGLRenderer : public Tbx::IRenderer
    {
    public:
        void Initialize(const std::shared_ptr<Tbx::IRenderSurface>& surface) override;

        void Process(const Tbx::FrameBuffer& buffer) override;

        void Flush() override;
        void Clear(const Tbx::Color& color) override;

        void SetApi(Tbx::GraphicsApi api) override;
        Tbx::GraphicsApi GetApi() override;

        void SetResolution(const Tbx::Size& size) override;
        Tbx::Size GetResolution() override;

        void SetViewport(const Tbx::Viewport& viewPort) override;
        Tbx::Viewport GetViewport() override;

        void SetVSyncEnabled(bool enabled) override;
        bool GetVSyncEnabled() override;

        void Draw(const Tbx::Mesh& mesh);

        void UploadMaterial(const Tbx::Material& material);
        void UploadTexture(const Tbx::Texture& texture, const Tbx::uint& slot);
        void UploadShaderUniform(const Tbx::ShaderUniform& data);

    private:
        Tbx::Uid _activeMaterial = Tbx::Invalid::Uid;
        std::unordered_map<Tbx::Uid, OpenGLMaterial> _materialCache = {};
        Tbx::Size _resolution = {};
        Tbx::Viewport _viewport = {};
        OpenGLContext _context = {};
        bool _vsyncEnabled = false;
    };
}

