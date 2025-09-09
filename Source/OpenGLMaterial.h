#pragma once
#include "IBindable.h"
#include "OpenGLTexture.h"
#include <Tbx/Graphics/Material.h>
#include <Tbx/Debug/Debugging.h>
#include <glad/glad.h>

namespace OpenGLRendering
{
    static GLenum ShaderUniformTypeToOpenGLType(const Tbx::ShaderUniformDataType& type)
    {
        using enum Tbx::ShaderUniformDataType;
        switch (type)
        {
            case None:     return GL_NONE;
            case Float:    return GL_FLOAT;
            case Float2:   return GL_FLOAT;
            case Float3:   return GL_FLOAT;
            case Float4:   return GL_FLOAT;
            case Mat3:     return GL_FLOAT;
            case Mat4:     return GL_FLOAT;
            case Int:      return GL_INT;
            case Int2:     return GL_INT;
            case Int3:     return GL_INT;
            case Int4:     return GL_INT;
            case Bool:     return GL_BOOL;
        }

        TBX_ASSERT(false, "Couln not convert to OpenGL type from ShaderDataType, given unknown ShaderDataType!");
        return GL_NONE;
    }

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

