#include "dpch.h"
#include "Platform/OpenGL/OpenGLRendererApi.h"

#include <glad/glad.h>

namespace Dwarf {
    OpenGLRendererApi::OpenGLRendererApi(){}
    OpenGLRendererApi::~OpenGLRendererApi(){}

    void OpenGLRendererApi::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_LINE_SMOOTH);
	}

    void OpenGLRendererApi::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

    void OpenGLRendererApi::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererApi::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}