#pragma once

// A GLAD-compatible loader signature:
using GLGetProcAddr = void*(*)(const char* name);

// Returns a function pointer for an OpenGL symbol on the current platform.
// Safe to pass directly to gladLoadGLLoader(GetOpenGLProcAddress).
void* GetOpenGLProcAddress(const char* name);