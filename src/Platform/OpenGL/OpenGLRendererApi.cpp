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
		glEnable(GL_LINE_SMOOTH);
		SetViewport(0,0,512,512);
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

	void OpenGLRendererApi::RenderIndexed(Ref<Mesh> mesh, Ref<Material> material, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
	{
		Ref<OpenGLMesh> oglMesh = CreateRef<OpenGLMesh>(*static_cast<OpenGLMesh*>(mesh.get()));
		Ref<OpenGLShader> oglShader = CreateRef<OpenGLShader>(*static_cast<OpenGLShader*>(material->GetShader().get()));

		glUseProgram(oglShader->GetID());
		GLuint mmID = glGetUniformLocation(oglShader->GetID(), "modelMatrix");
		GLuint vmID = glGetUniformLocation(oglShader->GetID(), "viewMatrix");
		GLuint pmID = glGetUniformLocation(oglShader->GetID(), "projectionMatrix");

		glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(vmID, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(pmID, 1, GL_FALSE, &projectionMatrix[0][0]);
		oglMesh->Bind();

		glDrawElements(GL_TRIANGLES, oglMesh->m_IndexCount, GL_UNSIGNED_INT, 0);

		oglMesh->Unbind();
		glUseProgram(0);
	}
}