#include "dpch.h"
#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Core/Asset/AssetDatabase.h"

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
		Ref<OpenGLMesh> oglMesh = std::dynamic_pointer_cast<OpenGLMesh>(mesh);
		Ref<OpenGLShader> shader = std::dynamic_pointer_cast<OpenGLShader>(material->GetShader());
		char textureInputCounter = 0;

		glUseProgram(shader->GetID());

		if (material->IsTransparent()) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		for(auto const& [key, val] : material->GetTextures()){
			if(val){
				glActiveTexture(GL_TEXTURE0+textureInputCounter);
				glBindTexture(GL_TEXTURE_2D, AssetDatabase::Retrieve<TextureAsset>(val)->GetAsset()->m_Texture->GetTextureID());

				GLuint uniformID = glGetUniformLocation(shader->GetID(), key.c_str());
				glUniform1i(uniformID, textureInputCounter);
			}

			textureInputCounter++;
		}

		GLuint uniformID;
		// Bind booleans
		for(auto const& [key, val] : material->GetBoolUniforms()){
			uniformID = glGetUniformLocation(shader->GetID(), key.c_str());
			glUniform1f(uniformID, (float)val);
		}

		// Bind integers
		for(auto const& [key, val] : material->GetIntegerUniforms()){
			uniformID = glGetUniformLocation(shader->GetID(), key.c_str());
			glUniform1i(uniformID, val);
		}

		// Bind floats
		for(auto const& [key, val] : material->GetFloatUniforms()){
			uniformID = glGetUniformLocation(shader->GetID(), key.c_str());
			glUniform1f(uniformID, val);
		}

		// Bind vector2
		for(auto const& [key, val] : material->Get2DUniforms()){
			uniformID = glGetUniformLocation(shader->GetID(), key.c_str());
			glUniform2f(uniformID, val.x, val.y);
		}

		// Bind vector3
		for(auto const& [key, val] : material->Get3DUniforms()){
			uniformID = glGetUniformLocation(shader->GetID(), key.c_str());
			glUniform3f(uniformID, val.x, val.y, val.z);
		}

		// Bind vector4
		for(auto const& [key, val] : material->Get4DUniforms()){
			uniformID = glGetUniformLocation(shader->GetID(), key.c_str());
			//glUniform4fv(uniformID, 4, &val.x);
			glUniform4f(uniformID, val.x, val.y, val.z, val.w);
		}

		GLuint mmID = glGetUniformLocation(shader->GetID(), "modelMatrix");
		GLuint vmID = glGetUniformLocation(shader->GetID(), "viewMatrix");
		GLuint pmID = glGetUniformLocation(shader->GetID(), "projectionMatrix");

		glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(vmID, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(pmID, 1, GL_FALSE, &projectionMatrix[0][0]);
		oglMesh->Bind();

		glDrawElements(GL_TRIANGLES, oglMesh->m_IndexCount, GL_UNSIGNED_INT, 0);

		oglMesh->Unbind();

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glUseProgram(0);
	}
}