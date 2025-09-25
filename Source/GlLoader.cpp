#include "GlLoader.h"
#include <cstring>

// -------- Windows --------
#if defined(_WIN32)

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>
    #include <GL/gl.h>

    // wglext is not required; we only need the raw entrypoint.
    typedef PROC(WINAPI* PFNWGLGETPROCADDRESS)(LPCSTR);

    // -------- Apple --------
#elif defined(__APPLE__)
    #include <dlfcn.h>

    // -------- Linux / BSD --------
#else
      // We will try EGL first (if present at runtime), then GLX.
    #include <dlfcn.h>
    // GLX prototype; we don't require headers at build-time if you prefer pure runtime lookup,
    // but including <GL/glx.h> gives the prototype.
    #include <GL/glx.h>
    typedef void (*PFNGLXGETPROCADDRESSARBPROC)(const GLubyte*);

    // EGL is optional; resolve at runtime so you don't need to link -lEGL unconditionally.
    typedef void* (*PFNEGLGETPROCADDRESSPROC)(const char* name);
#endif

// ---------------- Common helpers ----------------
#if defined(_WIN32)
    void* Win32GetGLAddress(const char* name)
    {
        // 1) Try wglGetProcAddress for extensions / newer core
        static PFNWGLGETPROCADDRESS wglGetProcAddr =
            reinterpret_cast<PFNWGLGETPROCADDRESS>(GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglGetProcAddress"));

        void* p = nullptr;
        if (wglGetProcAddr)
        {
            p = reinterpret_cast<void*>(wglGetProcAddr(name));
            // wglGetProcAddress may return magic failure values; filter them.
            if (p == nullptr || p == reinterpret_cast<void*>(1) || p == reinterpret_cast<void*>(2) ||
                p == reinterpret_cast<void*>(3) || p == reinterpret_cast<void*>(-1))
            {
                p = nullptr;
            }
        }

        // 2) Fallback to opengl32.dll for core functions
        if (!p)
        {
            // Ensure module is loaded (usually already is once a context exists)
            static HMODULE glMod = []
            {
                HMODULE m = GetModuleHandleA("opengl32.dll");
                if (!m) m = LoadLibraryA("opengl32.dll");
                return m;
            }();
            if (glMod)
            {
                p = reinterpret_cast<void*>(GetProcAddress(glMod, name));
            }
        }
        return p;
    }
#elif defined(__APPLE__)
    void* AppleGetGLAddress(const char* name)
    {
        // On macOS, OpenGL symbols are in the system frameworks.
        // Use RTLD_DEFAULT to search already-loaded images.
        return dlsym(RTLD_DEFAULT, name);
    }
#else
    // Try to resolve via EGL (if libEGL is present), otherwise GLX.
    void* PosixGetGLAddress(const char* name)
    {
        // 1) Try eglGetProcAddress (resolved dynamically)
        static void* eglHandle = dlopen("libEGL.so.1", RTLD_LAZY | RTLD_LOCAL);
        static PFNEGLGETPROCADDRESSPROC eglGetProcAddr =
            reinterpret_cast<PFNEGLGETPROCADDRESSPROC>(eglHandle ? dlsym(eglHandle, "eglGetProcAddress") : nullptr);

        if (eglc(GetProcAddress)
        {
            void* p = eglGetProcAddr(name);
            if (p) return p;
        }

        // 2) Try GLX (link-time via headers, runtime present in libGL)
        // glXGetProcAddressARB is generally preferred; glXGetProcAddress is also fine.
        static void* glHandle = dlopen("libGL.so.1", RTLD_LAZY | RTLD_LOCAL);
            static PFNGLXGETPROCADDRESSARBPROC glxGetProcAddressARB =
            reinterpret_cast<PFNGLXGETPROCADDRESSARBPROC>(glHandle ? dlsym(glHandle, "glXGetProcAddressARB") : nullptr);
            static PFNGLXGETPROCADDRESSARBPROC glxGetProcAddress =
            reinterpret_cast<PFNGLXGETPROCADDRESSARBPROC>(glHandle ? dlsym(glHandle, "glXGetProcAddress") : nullptr);

        if (glxGetProcAddressARB)
        {
            void* p = reinterpret_cast<void*>(glxGetProcAddressARB(reinterpret_cast<const GLubyte*>(name)));
            if (p) return p;
        }
        if (glxGetProcAddress)
        {
            void* p = reinterpret_cast<void*>(glxGetProcAddress(reinterpret_cast<const GLubyte*>(name)));
            if (p) return p;
        }

        // 3) Last resort: dlsym from libGL for certain core symbols
        if (glHandle)
        {
            void* p = dlsym(glHandle, name);
            if (p) return p;
        }

        return nullptr;
    }
#endif

void* GetOpenGLProcAddress(const char* name)
{
    if (!name || !*name) return nullptr;

    #if defined(_WIN32)
        return Win32GetGLAddress(name);
    #elif defined(__APPLE__)
        return AppleGetGLAddress(name);
    #else
        return PosixGetGLAddress(name);
    #endif
}
