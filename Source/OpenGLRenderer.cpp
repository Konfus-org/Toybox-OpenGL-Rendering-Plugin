#include "OpenGLRenderer.h"
#include <Tbx/Debug/Debugging.h>
#include <glad/glad.h>

#define TBX_VERBOSE_ENABLED

namespace OpenGLRendering
{
    void OpenGLRenderer::Process(const Tbx::RenderCommandBuffer& buffer)
    {
        // Process all draw commands in frame buffer
        for (const auto& cmd : buffer.Commands)
        {
            switch (cmd.Type)
            {
                case Tbx::RenderCommandType::Clear:
                {
                    TBX_TRACE_VERBOSE("GL Rendering: Clearing color buffer");

                    const auto& color = std::any_cast<const Tbx::RgbaColor&>(cmd.Payload);
                    Clear(color);
                    break;
                }
                case Tbx::RenderCommandType::UploadMaterial:
                {
                    TBX_TRACE_VERBOSE("GL Rendering: Compiling material");

                    const auto& material = std::any_cast<const Tbx::MaterialInstance&>(cmd.Payload);
                    UploadMaterial(material);
                    break;
                }
                case Tbx::RenderCommandType::SetMaterial:
                {
                    TBX_TRACE_VERBOSE("GL Rendering: Setting material");

                    const auto& material = std::any_cast<const Tbx::MaterialInstance&>(cmd.Payload);
                    SetMaterial(material);
                    break;
                }
                case Tbx::RenderCommandType::SetUniform:
                {
                    TBX_TRACE_VERBOSE("GL Rendering: Uploading material data");

                    const auto& uniform = std::any_cast<const Tbx::ShaderUniform&>(cmd.Payload);
                    SetUniform(uniform);
                    break;
                }
                case Tbx::RenderCommandType::UploadMesh:
                {
                    TBX_TRACE_VERBOSE("GL Rendering: Uploading mesh");

                    const auto& mesh = std::any_cast<const Tbx::Mesh&>(cmd.Payload);
                    UploadMesh(mesh);
                    break;
                }
                case Tbx::RenderCommandType::DrawMesh:
                {
                    TBX_TRACE_VERBOSE("GL Rendering: Drawing mesh");

                    const auto& mesh = std::any_cast<const Tbx::Mesh&>(cmd.Payload);
                    DrawMesh(mesh);
                    break;
                }
                case Tbx::RenderCommandType::SetResolution:
                {
                    TBX_TRACE_VERBOSE("GL Rendering: Setting resolution");

                    const auto& resolution = std::any_cast<const Tbx::Size&>(cmd.Payload);
                    SetResolution(resolution);
                    break;
                }
                case Tbx::RenderCommandType::SetViewport:
                {
                    TBX_TRACE_VERBOSE("GL Rendering: Setting viewport");

                    const auto& viewport = std::any_cast<const Tbx::Viewport&>(cmd.Payload);
                    SetViewport(viewport);
                    break;
                }
                default:
                {
                    TBX_ASSERT(false, "GL Rendering: Unknown draw command type!");
                    break;
                }
            }
        }

        FinalizeFrame();
    }

    void OpenGLRenderer::Flush()
    {
        _meshCache.clear();
        _materialInstanceCache.clear();
        _materialCache.clear();
        Clear(Tbx::RgbaColor::Black);
    }

    void OpenGLRenderer::Clear(const Tbx::RgbaColor& color)
    {
        glClearColor(color.R, color.G, color.B, color.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        glViewport((GLint)viewport.Position.X, (GLint)viewport.Position.Y, viewport.Extends.Width, viewport.Extends.Height);
    }

    Tbx::Viewport OpenGLRenderer::GetViewport()
    {
        return _viewport;
    }

    void OpenGLRenderer::FinalizeFrame()
    {
        _materialInstanceCache[_activeMaterial].Unbind();
    }

    void OpenGLRenderer::DrawMesh(const Tbx::Mesh& mesh)
    {
        const auto& meshId = mesh.Id;
        auto& glMesh = _meshCache[meshId];
        glMesh.Bind();
        glDrawElements(GL_TRIANGLES, glMesh.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, 0);
    }

    void OpenGLRenderer::UploadMesh(const Tbx::Mesh& mesh)
    {
        // TODO: Implement mesh instancing
        const auto& meshId = mesh.Id;
        if (!_meshCache.contains(meshId))
        {
            _meshCache.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(meshId),
                std::forward_as_tuple());
            auto& glMesh = _meshCache[meshId];
            glMesh.Bind();
            glMesh.UploadVertexBuffer(mesh.Vertices);
            glMesh.UploadIndexBuffer(mesh.Indices);
        }
    }

    void OpenGLRenderer::SetMaterial(const Tbx::MaterialInstance& mat)
    {
        _materialInstanceCache[_activeMaterial].Unbind();
        _activeMaterial = mat.Id;
        _materialInstanceCache[_activeMaterial].Bind();
    }

    void OpenGLRenderer::UploadMaterial(const Tbx::MaterialInstance& materialInstance)
    {
        const auto& materialId = materialInstance.Material->Id;
        if (!_materialCache.contains(materialId))
        {
            _materialCache.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(materialId),
                std::forward_as_tuple());
            _materialCache[materialId].Upload(*materialInstance.Material);
        }

        const auto& materialInstanceId = materialInstance.Id;
        if (!_materialInstanceCache.contains(materialInstanceId))
        {
            _materialInstanceCache.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(materialInstanceId),
                std::forward_as_tuple(_materialCache[materialId]));
            _materialInstanceCache[materialInstanceId].Upload(materialInstance);
        }
    }

    void OpenGLRenderer::SetUniform(const Tbx::ShaderUniform& data)
    {
        const auto& mat = _materialInstanceCache[_activeMaterial];
        mat.SetUniform(data);
    }
}
