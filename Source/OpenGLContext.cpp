#include "OpenGLContext.h"
#include <Tbx/Debug/Debugging.h>
#include <glad/glad.h>

namespace OpenGLRendering
{
    void GLAPIENTRY GlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                TBX_ASSERT(false, "GL CALLBACK: type = {}, severity = {}, message = {}\n", type, severity, message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                TBX_TRACE_ERROR("GL CALLBACK: type = {}, severity = {}, message = {}\n", type, severity, message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                TBX_TRACE_WARNING("GL CALLBACK: type = {}, severity = {}, message = {}\n", type, severity, message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                TBX_TRACE_INFO("GL CALLBACK: type = {}, severity = {}, message = {}\n", type, severity, message);
                break;
            default:
                TBX_TRACE_WARNING("GL CALLBACK: type = {}, severity = {}, message = {}\n", severity, message);
                break;
        }
    }

    void OpenGLContext::Set(const std::weak_ptr<Tbx::IRenderSurface>& surface)
    {
        _renderSurface = surface;

        int gladStatus = gladLoadGLLoader((GLADloadproc)_renderSurface.lock()->GetProcAddress());
        TBX_ASSERT(gladStatus, "Failed to initialize Glad!");

        TBX_TRACE_INFO("OpenGL Info:");
        const std::string& vendorVersion = (const char*)glGetString(GL_VENDOR);
        TBX_TRACE_INFO("  Vendor: {0}", vendorVersion);
        const std::string& rendererVersion = (const char*)glGetString(GL_RENDERER);
        TBX_TRACE_INFO("  Renderer: {0}", rendererVersion);
        const std::string& glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        TBX_TRACE_INFO("  GLSL: {0}", glslVersion);
        const std::string& openGLVersion = (const char*)glGetString(GL_VERSION);
        TBX_TRACE_INFO("  Version: {0}", openGLVersion);

        TBX_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Tbx requires at least OpenGL version 4.5!");

#ifdef TBX_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(
            GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DEBUG_SEVERITY_NOTIFICATION,
            0,
            nullptr,
            GL_FALSE);
        glDebugMessageCallback(GlMessageCallback, 0);
#endif

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    void OpenGLContext::SwapBuffers()
    {
        if (_renderSurface.expired() || _renderSurface.lock() == nullptr)
        {
            return;
        }

        _renderSurface.lock()->SwapBuffers();
    }

    int OpenGLContext::GetSwapInterval() const
    {
        if (_renderSurface.expired() || _renderSurface.lock() == nullptr)
        {
            return 0;
        }

        return _renderSurface.lock()->GetSwapInterval();
    }

    void OpenGLContext::SetSwapInterval(int interval) const
    {
        if (_renderSurface.expired() || _renderSurface.lock() == nullptr)
        {
            return;
        }

        _renderSurface.lock()->SetSwapInterval(interval);
    }

    std::weak_ptr<Tbx::IRenderSurface> OpenGLContext::GetRenderSurface()
    {
        return _renderSurface;
    }
}
