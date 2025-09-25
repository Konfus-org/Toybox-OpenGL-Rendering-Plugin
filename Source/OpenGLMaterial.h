#pragma once
#include "IBindable.h"
#include "OpenGLTexture.h"
#include <Tbx/Graphics/Material.h>

namespace OpenGLRendering
{
    class OpenGLShader
    {
    public:
        ~OpenGLShader();
        void Compile(const Tbx::Shader& shader, Tbx::uint programId);
        void UploadUniform(const Tbx::ShaderUniform& data) const;
        void Detach() const;

    private:
        Tbx::uint _programId = -1;
        Tbx::uint _shaderId = -1;
        Tbx::ShaderType _type = Tbx::ShaderType::None;
    };

    class OpenGLMaterial : public IBindable
    {
    public:
        OpenGLMaterial() = default;
        ~OpenGLMaterial() final;

        void Upload(const Tbx::Material& material);
        void UploadUniform(const Tbx::ShaderUniform& data) const;
        void Bind() const final;
        void Unbind() const final;

    private:
        Tbx::uint _materialGLId = -1;
        std::vector<OpenGLShader> _shaders = {};
    };

    static OpenGLMaterial DefaultMaterial = {};

    class OpenGLMaterialInstance : public IBindable
    {
    public:
        OpenGLMaterialInstance()
            : _material(DefaultMaterial) {}
        OpenGLMaterialInstance(const OpenGLMaterial& material)
            : _material(material) {}

        void Upload(const Tbx::MaterialInstance& material);
        void SetUniform(const Tbx::ShaderUniform& data) const;
        void Bind() const final;
        void Unbind() const final;

    private:
        const OpenGLMaterial& _material;
        std::vector<OpenGLTexture> _textures = {};
    };
}

