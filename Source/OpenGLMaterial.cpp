#include "OpenGLMaterial.h"
#include <Tbx/Math/Vectors.h>
#include <Tbx/Math/Mat4x4.h>
#include <vector>

namespace OpenGLRendering
{
    ///// Helpers //////////////////////////////////////////////////////////////////
    static void UploadUniformInt(const std::string& name, int value, Tbx::uint rendererId)
    {
        GLint location = glGetUniformLocation(rendererId, name.c_str());
        glUniform1i(location, value);
    }

    static void UploadUniformIntArray(const std::string& name, std::vector<int> values, Tbx::uint rendererId)
    {
        GLint location = glGetUniformLocation(rendererId, name.c_str());
        glUniform1iv(location, (Tbx::uint32)values.size(), values.data());
    }

    static void UploadUniformFloat(const std::string& name, float value, Tbx::uint rendererId)
    {
        GLint location = glGetUniformLocation(rendererId, name.c_str());
        glUniform1f(location, value);
    }

    static void UploadUniformFloat2(const std::string& name, const Tbx::Vector2& value, Tbx::uint rendererId)
    {
        GLint location = glGetUniformLocation(rendererId, name.c_str());
        glUniform2f(location, value.X, value.Y);
    }

    static void UploadUniformFloat3(const std::string& name, const Tbx::Vector3& value, Tbx::uint rendererId)
    {
        GLint location = glGetUniformLocation(rendererId, name.c_str());
        glUniform3f(location, value.X, value.Y, value.Z);
    }

    static void UploadUniformFloat4(const std::string& name, const Tbx::Color& value, Tbx::uint rendererId)
    {
        GLint location = glGetUniformLocation(rendererId, name.c_str());
        glUniform4f(location, value.R, value.G, value.B, value.A);
    }

    // TODO: Implement when needed... Toybox doesn't have a mat3 yet...
    ////static void UploadUniformMat3(const std::string& name, const Tbx::Matrix& matrix, Tbx::uint rendererId)
    ////{
    ////	GLint location = glGetUniformLocation(rendererId, name.c_str());
    ////	glUniformMatrix3fv(location, 1, GL_FALSE, matrix.Values.data());
    ////}

    static void UploadUniformMat4(const std::string& name, const Tbx::Mat4x4& matrix, Tbx::uint rendererId)
    {
        GLint location = glGetUniformLocation(rendererId, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.Values.data());
    }

    ///// Shader //////////////////////////////////////////////////////////////////

    void OpenGLShader::Compile(const Tbx::Shader& shader, Tbx::uint rendererId)
    {
        // The code below is a lighty modified version of the example code found here:
        // https://www.khronos.org/opengl/wiki/Shader_Compilation

        // Create a shader handle
        GLuint glShader;
        if (shader.GetType() == Tbx::ShaderType::Vertex)
        {
            glShader = glCreateShader(GL_VERTEX_SHADER);
        }
        else if (shader.GetType() == Tbx::ShaderType::Fragment)
        {
            glShader = glCreateShader(GL_FRAGMENT_SHADER);
        }
        else
        {
            TBX_ASSERT(false, "Unsupported shader type.");
        }

        // Send the shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const auto* source = shader.GetSource().c_str();
        glShaderSource(glShader, 1, &source, nullptr);

        // Compile the vertex shader
        glCompileShader(glShader);

        GLint isCompiled = 0;
        glGetShaderiv(glShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(glShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(glShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(glShader);

            const auto& error = std::string(infoLog.data());
            TBX_ASSERT(false, "Shader compilation failure: {}", error);

            // In this simple program, we'll just leave
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        _rendererId = rendererId;

        // Attach our shaders to our program
        glAttachShader(_rendererId, glShader);

        // Link our program
        glLinkProgram(_rendererId);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(_rendererId, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(_rendererId, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(_rendererId, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(_rendererId);
            // Don't leak shaders either.
            glDeleteShader(glShader);

            const auto& error = std::string(infoLog.data());
            TBX_ASSERT(false, "Shader link failure: {0}", error);

            // In this simple program, we'll just leave
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(_rendererId, glShader);
    }

    void OpenGLShader::UploadUniform(const Tbx::ShaderUniform& data) const
    {
        switch (data.DataType)
        {
            case Tbx::ShaderUniformDataType::Mat4:
            {
                UploadUniformMat4(data.Name, std::any_cast<Tbx::Mat4x4>(data.Data), _rendererId);
                break;
            }
            case Tbx::ShaderUniformDataType::Float:
            {
                UploadUniformFloat(data.Name, std::any_cast<float>(data.Data), _rendererId);
                break;
            }
            case Tbx::ShaderUniformDataType::Float2:
            {
                UploadUniformFloat2(data.Name, std::any_cast<Tbx::Vector2>(data.Data), _rendererId);
                break;
            }
            case Tbx::ShaderUniformDataType::Float3:
            {
                UploadUniformFloat3(data.Name, std::any_cast<Tbx::Vector3>(data.Data), _rendererId);
                break;
            }
            case Tbx::ShaderUniformDataType::Float4:
            {
                UploadUniformFloat4(data.Name, std::any_cast<Tbx::Color>(data.Data), _rendererId);
                break;
            }
            case Tbx::ShaderUniformDataType::Int:
            {
                UploadUniformInt(data.Name, std::any_cast<int>(data.Data), _rendererId);
                break;
            }
            default:
            {
                TBX_ASSERT(false, "Unsupported shader data type.");
                break;
            }
        }
    }

    ///// Material //////////////////////////////////////////////////////////////////

    OpenGLMaterial::~OpenGLMaterial()
    {
        glUseProgram(0);
        glDeleteProgram(_rendererId);
    }

    void OpenGLMaterial::Upload(const Tbx::Material& material)
    {
        _rendererId = glCreateProgram();
        for (const auto& shader : material.GetShaders())
        {
            auto& glShader = _shaders.emplace_back();
            glShader.Compile(shader, _rendererId);
        }

        Tbx::uint slot = 0;
        for (const auto& texture : material.GetTextures())
        {
            auto& glTexture = _textures.emplace_back();
            glTexture.Upload(texture, slot);
            slot++;
        }
    }

    void OpenGLMaterial::Bind() const
    {
        glUseProgram(_rendererId);

        for (const auto& tex : _textures)
        {
            tex.Bind();
        }
    }

    void OpenGLMaterial::Unbind() const
    {
        glUseProgram(0);

        for (const auto& tex : _textures)
        {
            tex.Unbind();
        }
    }

    void OpenGLMaterial::UploadUniform(const Tbx::ShaderUniform& data) const
    {
        for (const auto& shader : _shaders)
        {
            shader.UploadUniform(data);
        }
    }

}
