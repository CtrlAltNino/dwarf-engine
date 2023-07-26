#include "dpch.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Forward/ForwardRenderer.h"
#include "Core/Rendering/Deferred/DeferredRenderer.h"

namespace Dwarf {

	GraphicsApi Renderer::s_Api = GraphicsApi::OpenGL;

	Renderer::RendererType Renderer::s_RendererType = Renderer::RendererType::Forward;

	GraphicsApi Renderer::GetAPI() { return s_Api; }

	Renderer::RendererType Renderer::GetType() { return s_RendererType; }

	Ref<Renderer> Renderer::s_Renderer = nullptr;

	void Renderer::Create(GraphicsApi api, Renderer::RendererType type){
		s_Api = api;
		switch(type){
			case Renderer::RendererType::Forward:
					s_Renderer = CreateRef<ForwardRenderer>();
				break;
			case Renderer::RendererType::Deferred:
					s_Renderer = CreateRef<DeferredRenderer>();
				break;
		}
	}

	// ========== Renderer Functions ==========

	//void Renderer::render(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix) {
		//material.bind();
		//material.UpdateMVP(modelMatrix, viewMatrix, projectionMatrix);
		//material.UpdateUniforms();
		//std::cout << "Model matrix" << std::endl;
		//material->updateShaderParameters(modelMatrix, viewMatrix, projectionMatrix);

		/*if (material.settings.isTransparent) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		if (material.settings.cullFaces) {
			glEnable(GL_CULL_FACE);
			glCullFace(material.settings.cullMode);
		}


		for (int i = 0; i < meshes.size(); i++) {
			meshes.at(i).bind();
			glDrawElements(GL_TRIANGLES, meshes.at(i).indices2.size(), GL_UNSIGNED_INT, 0);
			meshes.at(i).unbind();
		}

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);*/

		/*if (material.m_Transparent) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);*/

		/*if (material.settings.cullFaces) {
			glEnable(GL_CULL_FACE);
			glCullFace(material.settings.cullMode);
		}*/


		/*for (int i = 0; i < meshes.size(); i++) {
			meshes.at(i).bind();
			glDrawElements(GL_TRIANGLES, meshes.at(i).indices2.size(), GL_UNSIGNED_INT, 0);
			meshes.at(i).unbind();
		}

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);*/
	//}
}