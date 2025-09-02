#include "OpenGLRenderer.h"
#include <Tbx/App/App.h>

#define TBX_VERBOSE_ENABLED

namespace OpenGLRendering
{
    void OpenGLRenderer::Initialize(const std::shared_ptr<Tbx::IRenderSurface>& surface)
    {
        _context.Set(surface);
    }

    void OpenGLRenderer::Process(const Tbx::FrameBuffer& buffer)
    {
        // Process all draw commands in frame buffer
        for (const auto& cmd : buffer.GetCommands())
        {
            switch (cmd.GetType())
            {
                case Tbx::DrawCommandType::Clear:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Clearing color buffer");

                    const auto& color = std::any_cast<const Tbx::Color&>(cmd.GetPayload());
                    Clear(color);
                    break;
                }
                case Tbx::DrawCommandType::CompileMaterial:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Compiling material");

                    const auto& material = std::any_cast<const Tbx::MaterialInstance&>(cmd.GetPayload());
                    UploadMaterial(material);
                    break;
                }
                case Tbx::DrawCommandType::SetMaterial:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Setting material");

                    const auto& material = std::any_cast<const Tbx::MaterialInstance&>(cmd.GetPayload());
                    SetMaterial(material);
                    break;
                }
                case Tbx::DrawCommandType::UploadUniform:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Uploading material data");

                    const auto& uniform = std::any_cast<const Tbx::ShaderUniform&>(cmd.GetPayload());
                    UploadShaderUniform(uniform);
                    break;
                }
                case Tbx::DrawCommandType::UploadMesh:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Uploading mesh");

                    const auto& mesh = std::any_cast<const Tbx::Mesh&>(cmd.GetPayload());
                    UploadMesh(mesh);
                    break;
                }
                case Tbx::DrawCommandType::DrawMesh:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Drawing mesh");

                    const auto& mesh = std::any_cast<const Tbx::Mesh&>(cmd.GetPayload());
                    DrawMesh(mesh);
                    break;
                }
                default:
                {
                    TBX_ASSERT(false, "Unknown draw command type!");
                    break;
                }
            }
        }

        FinalizeFrame();
    }

    void OpenGLRenderer::Flush()
    {
        _materialCache.clear();
        auto clearColor = Tbx::App::GetInstance()->GetGraphicsSettings().ClearColor;
        Clear(clearColor);
    }

    void OpenGLRenderer::Clear(const Tbx::Color& color)
    {
        glClearColor(color.R, color.G, color.B, color.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderer::SetApi(Tbx::GraphicsApi api)
    {
        TBX_ASSERT(api == Tbx::GraphicsApi::OpenGL, "OpenGL is the only API currently supported!");
    }

    Tbx::GraphicsApi OpenGLRenderer::GetApi()
    {
        return Tbx::GraphicsApi::OpenGL;
    }

    void OpenGLRenderer::SetResolution(const Tbx::Size& size)
    {
        _resolution = size;
    }

    Tbx::Size OpenGLRenderer::GetResolution()
    {
        return _resolution;
    }

    void OpenGLRenderer::SetViewport(const Tbx::Viewport& viewport)
    {
        _viewport = viewport;
        glViewport((GLint)viewport.Position.X, (GLint)viewport.Position.Y, viewport.Size.Width, viewport.Size.Height);
    }

    Tbx::Viewport OpenGLRenderer::GetViewport()
    {
        return _viewport;
    }

    void OpenGLRenderer::SetVSyncEnabled(bool enabled)
    {
        _vsyncEnabled = enabled;
        _context.SetSwapInterval(enabled);
    }

    bool OpenGLRenderer::GetVSyncEnabled()
    {
        return _vsyncEnabled;
    }

    void OpenGLRenderer::FinalizeFrame()
    {
        _materialInstanceCache[_activeMaterial].Unbind();
    }

    void OpenGLRenderer::DrawMesh(const Tbx::Mesh& mesh)
    {
        const auto& meshId = mesh.GetId();
        auto& glMesh = _meshCache[meshId];
        glMesh.Bind();
        glDrawElements(GL_TRIANGLES, glMesh.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, 0);
    }

    void OpenGLRenderer::UploadMesh(const Tbx::Mesh& mesh)
    {
        // TODO: Implement mesh instancing
        const auto& meshId = mesh.GetId();
        if (!_meshCache.contains(meshId))
        {
            _meshCache.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(meshId),
                std::forward_as_tuple());
            auto& glMesh = _meshCache[meshId];
            glMesh.Bind();
            glMesh.UploadVertexBuffer(mesh.GetVertexBuffer());
            glMesh.UploadIndexBuffer(mesh.GetIndexBuffer());
        }
    }

    void OpenGLRenderer::SetMaterial(const Tbx::MaterialInstance& mat)
    {
        _materialInstanceCache[_activeMaterial].Unbind();
        _activeMaterial = mat.GetId();
        _materialInstanceCache[_activeMaterial].Bind();
    }

    void OpenGLRenderer::UploadMaterial(const Tbx::MaterialInstance& materialInstance)
    {
        const auto& materialId = materialInstance.GetMaterial().GetId();
        if (!_materialCache.contains(materialId))
        {
            _materialCache.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(materialId),
                std::forward_as_tuple());
            _materialCache[materialId].Upload(materialInstance.GetMaterial());
        }

        const auto& materialInstanceId = materialInstance.GetId();
        if (!_materialInstanceCache.contains(materialInstanceId))
        {
            _materialInstanceCache.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(materialInstanceId),
                std::forward_as_tuple(_materialCache[materialId]));
            _materialInstanceCache[materialInstanceId].Upload(materialInstance);
        }
    }

    void OpenGLRenderer::UploadShaderUniform(const Tbx::ShaderUniform& data)
    {
        const auto& mat = _materialInstanceCache[_activeMaterial];
        mat.UploadUniform(data);
    }
}
