#include "OpenGLRendererFactoryPlugin.h"
#include "OpenGLRenderer.h"
#include <Tbx/Debug/Debugging.h>
#include <SDL3/SDL.h>

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

    std::shared_ptr<Tbx::IRenderer> OpenGLRendererFactoryPlugin::Create()
    {
        if (!_isGlInitialized)
        {
            InitializeOpenGl();
        }

        auto renderer = std::shared_ptr<Tbx::IRenderer>(New(), [this](Tbx::IRenderer* renderer) { Delete(renderer); });
        return renderer;
    }

    void OpenGLRendererFactoryPlugin::InitializeOpenGl()
    {
        // Set attribute
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef TBX_DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
        // Validate attributes
        int att = 0;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &att);
        TBX_ASSERT(att == 4, "Failed to set OpenGL context major version to 4");
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &att);
        TBX_ASSERT(att == 5, "Failed to set OpenGL context minor version to 5");
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &att);
        TBX_ASSERT(att == SDL_GL_CONTEXT_PROFILE_CORE, "Failed to set OpenGL context profile to core");
#ifdef TBX_DEBUG
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &att);
        TBX_ASSERT(att == SDL_GL_CONTEXT_DEBUG_FLAG, "Failed to set OpenGL context debug flag");
#endif
        // Load the OpenGL functions using Glad
        // TODO: Make a sdl gl loader plugin that we use here!
        int gladStatus = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
        TBX_ASSERT(gladStatus, "Failed to initialize Glad!");

        // Print gl info
        TBX_TRACE_INFO("OpenGL Info:");
        const std::string& vendorVersion = (const char*)glGetString(GL_VENDOR);
        TBX_TRACE_INFO("  Vendor: {0}", vendorVersion);
        const std::string& rendererVersion = (const char*)glGetString(GL_RENDERER);
        TBX_TRACE_INFO("  Renderer: {0}", rendererVersion);
        const std::string& glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        TBX_TRACE_INFO("  GLSL: {0}", glslVersion);
        const std::string& openGLVersion = (const char*)glGetString(GL_VERSION);
        TBX_TRACE_INFO("  Version: {0}", openGLVersion);

        // Check OpenGL version
        TBX_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Tbx requires at least OpenGL version 4.5!");

#ifdef TBX_DEBUG
        // Enable debug output
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

        // Congifure global gl stuff
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDepthFunc(GL_LEQUAL);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        _isGlInitialized = true;
    }

    Tbx::IRenderer* OpenGLRendererFactoryPlugin::New()
    {
        auto* renderer = new OpenGLRenderer();
        return renderer;
    }

    void OpenGLRendererFactoryPlugin::Delete(Tbx::IRenderer* renderer)
    {
        delete renderer;
    }
}