#include "dpch.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include <glad/glad.h>
#include <SDL2/SDL_opengl.h>

namespace Dwarf
{

    OpenGLContext::OpenGLContext(SDL_Window *windowHandle) : m_WindowHandle(windowHandle) {}

    OpenGLContext::~OpenGLContext()
    {
        if (m_Context)
            SDL_GL_DeleteContext(m_Context);
    }

    void OpenGLContext::Init()
    {
        m_Context = SDL_GL_CreateContext(m_WindowHandle);

        SDL_GL_MakeCurrent(m_WindowHandle, m_Context);
        gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    }

    void OpenGLContext::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_WindowHandle);
    }
}
