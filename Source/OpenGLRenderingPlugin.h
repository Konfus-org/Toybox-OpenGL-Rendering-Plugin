#pragma once
#include <Tbx/Plugins/Plugin.h>
#include <Tbx/Graphics/GraphicsBackend.h>
#include <Tbx/Graphics/GraphicsResources.h>

namespace Tbx::Plugins::OpenGLRendering
{
    class OpenGLRenderingPlugin final
        : public Plugin
        , public IGraphicsBackend
    {
    public:
        OpenGLRenderingPlugin(Ref<EventBus> eventBus) {}

        GraphicsApi GetApi() const override;

        void SetContext(Ref<IGraphicsContext> context) override;
        void BeginDraw(const RgbaColor& clearColor, const Viewport& viewport) override;
        void EndDraw() override;

        Ref<TextureResource> UploadTexture(const Texture& texture) override;
        Ref<MeshResource> UploadMesh(const Mesh& mesh) override;

        Ref<ShaderProgramResource> CreateShaderProgram(const std::vector<Ref<ShaderResource>>& shadersToLink) override;
        Ref<ShaderResource> CompileShader(const Shader& shader) override;

    private:
        void InitializeOpenGl(Ref<IGraphicsContext> context);
        void DeleteResource(GraphicsResource* resourceToDelete);

    private:
        Ref<IGraphicsContext> _context = nullptr;
        bool _isGlInitialized = false;

        // Inherited via IGraphicsBackend
        void EnableDepthTesting(bool enabled) override;
    };

    TBX_REGISTER_PLUGIN(OpenGLRenderingPlugin);
}