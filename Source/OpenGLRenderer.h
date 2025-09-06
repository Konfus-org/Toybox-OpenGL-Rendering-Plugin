#pragma once
#include "OpenGLContext.h"
#include "OpenGLBuffers.h"
#include "OpenGLMaterial.h"
#include "OpenGLMesh.h"
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
        void Clear(const Tbx::RgbaColor& color) override;

        void SetApi(Tbx::GraphicsApi api) override;
        Tbx::GraphicsApi GetApi() override;

        void SetResolution(const Tbx::Size& size) override;
        Tbx::Size GetResolution() override;

        void SetViewport(const Tbx::Viewport& viewPort) override;
        Tbx::Viewport GetViewport() override;

        void SetVSyncEnabled(bool enabled) override;
        bool GetVSyncEnabled() override;

    private:
        void FinalizeFrame();

        void DrawMesh(const Tbx::Mesh& mesh);
        void UploadMesh(const Tbx::Mesh& mesh);

        void SetMaterial(const Tbx::MaterialInstance& mat);
        void UploadMaterial(const Tbx::MaterialInstance& materialInstance);

        void UploadTexture(const Tbx::Texture& texture, const Tbx::uint& slot);

        void UploadShaderUniform(const Tbx::ShaderUniform& data);

        Tbx::Uid _activeMaterial = Tbx::Invalid::Uid;
        std::unordered_map<Tbx::Uid, OpenGLMesh> _meshCache = {};
        std::unordered_map<Tbx::Uid, OpenGLMaterial> _materialCache = {};
        std::unordered_map<Tbx::Uid, OpenGLMaterialInstance> _materialInstanceCache = {};
        Tbx::Size _resolution = {};
        Tbx::Viewport _viewport = {};
        OpenGLContext _context = {};
        bool _vsyncEnabled = false;
    };
}

