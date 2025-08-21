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
        for (const auto& cmd : buffer.GetCommands())
        {
            const GLenum error = glGetError();
            TBX_ASSERT(error == GL_NO_ERROR, "OpenGL error : {}", error);

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

                    const auto& material = std::any_cast<const Tbx::Material&>(cmd.GetPayload());
                    UploadMaterial(material);
                    break;
                }
                case Tbx::DrawCommandType::SetMaterial:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Setting material");

                    const auto& material = std::any_cast<const Tbx::Material&>(cmd.GetPayload());
                    _activeMaterial = material.GetId();
                    _materialCache[_activeMaterial].Bind();
                    break;
                }
                case Tbx::DrawCommandType::UploadMaterialData:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Uploading material data");

                    const auto& uniform = std::any_cast<const Tbx::ShaderUniform&>(cmd.GetPayload());
                    UploadShaderUniform(uniform);
                    break;
                }
                case Tbx::DrawCommandType::DrawMesh:
                {
                    TBX_TRACE_VERBOSE("GL RENDERER: Drawing mesh");

                    const auto& mesh = std::any_cast<const Tbx::Mesh&>(cmd.GetPayload());
                    Draw(mesh);
                    break;
                }
                default:
                    break;
            }
        }
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
        _context.SwapBuffers();
    }

    void OpenGLRenderer::Draw(const Tbx::Mesh& mesh)
    {
        // TODO: we could prolly have some sorta mesh cache/instancing
        const auto& meshVertexBuffer = mesh.GetVertexBuffer();
        const auto& meshIndexBuffer = mesh.GetIndexBuffer();

        OpenGLMesh glMesh;
        glMesh.UploadVertexBuffer(meshVertexBuffer);
        glMesh.UploadIndexBuffer(meshIndexBuffer);
        glMesh.Bind();

        glDrawElements(GL_TRIANGLES, glMesh.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
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

    void OpenGLRenderer::UploadMaterial(const Tbx::Material& material)
    {
        auto materialId = material.GetId();
        _materialCache.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(materialId),
            std::forward_as_tuple());
        _materialCache[materialId].Upload(material);
    }

    void OpenGLRenderer::UploadShaderUniform(const Tbx::ShaderUniform& data)
    {
        const auto& mat = _materialCache[_activeMaterial];
        mat.UploadUniform(data);
    }
}
