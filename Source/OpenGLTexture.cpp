#include "OpenGLTexture.h"

namespace OpenGLRendering
{
    /////// Helpers ///////////////////////////////////

    struct GlTextureFormat
    {
        GLenum InternalFormat = 0;
        GLenum DataFormat = 0;
    };

    int TbxTexFilterToGLTexFilter(const Tbx::Texture& tex)
    {
        if (tex.GetFilter() == Tbx::TextureFilter::Nearest)
        {
            return GL_NEAREST;
        }
        else if (tex.GetFilter() == Tbx::TextureFilter::Linear)
        {
            return GL_LINEAR;
        }
        else
        {
            TBX_ASSERT(false, "Unsupported texture filter type!");
            return GL_LINEAR;
        }
    }

    int TbxTexWrapToGLTexWrap(const Tbx::Texture& tex)
    {
        if (tex.GetWrap() == Tbx::TextureWrap::Repeat)
        {
            return GL_REPEAT;
        }
        else if (tex.GetWrap() == Tbx::TextureWrap::MirroredRepeat)
        {
            return GL_MIRRORED_REPEAT;
        }
        else if (tex.GetWrap() == Tbx::TextureWrap::ClampToEdge)
        {
            return GL_CLAMP_TO_EDGE;
        }
        else
        {
            TBX_ASSERT(false, "Unsupported texture wrap type!");
            return GL_REPEAT;
        }
    }

    GlTextureFormat TbxTexFormatToGLTexFormat(const Tbx::Texture& tex)
    {
        const Tbx::uint32 channelCount = tex.GetChannels();
        if (channelCount == 4)
        {
            return { GL_RGBA8, GL_RGBA };
        }
        else if (channelCount == 3)
        {
            return { GL_RGB8, GL_RGB };
        }
        else
        {
            return { GL_RGB8, GL_RGB };
            TBX_ASSERT(false, "Unsupported texture format.");
        }
    }

    /////// OpenGLTexture ///////////////////////////////////

    OpenGLTexture::OpenGLTexture()
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &_textureGLId);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &_textureGLId);
    }

    void OpenGLTexture::Upload(const Tbx::Texture& tex, const Tbx::uint& slot)
    {
        _slot = slot;

        // Generate texture
        glGenTextures(1, &_textureGLId);
        glBindTexture(GL_TEXTURE_2D, _textureGLId);

        // Convert tbx texture to OpenGL texture
        GlTextureFormat format = TbxTexFormatToGLTexFormat(tex);
        auto filtering = TbxTexFilterToGLTexFilter(tex);
        auto wrapping = TbxTexWrapToGLTexWrap(tex);
        glTextureParameteri(_textureGLId, GL_TEXTURE_MIN_FILTER, filtering);
        glTextureParameteri(_textureGLId, GL_TEXTURE_MAG_FILTER, filtering);
        glTextureParameteri(_textureGLId, GL_TEXTURE_WRAP_S, wrapping);
        glTextureParameteri(_textureGLId, GL_TEXTURE_WRAP_T, wrapping);

        // Upload texture data to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, format.InternalFormat, tex.GetWidth(), tex.GetHeight(), 0, format.DataFormat, GL_UNSIGNED_BYTE, tex.GetPixels().data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void OpenGLTexture::Bind() const
    {
        glBindTextureUnit(_slot, _textureGLId);
    }

    void OpenGLTexture::Unbind() const
    {
        glBindTextureUnit(_slot, 0);
    }
}