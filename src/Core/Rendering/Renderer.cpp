#include "dpch.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Forward/ForwardRenderer.h"
#include "Core/Rendering/Deferred/DeferredRenderer.h"

namespace Dwarf
{

	GraphicsApi Renderer::s_Api = GraphicsApi::OpenGL;

	Renderer::RendererType Renderer::s_RendererType = Renderer::RendererType::Forward;

	GraphicsApi Renderer::GetAPI() { return s_Api; }

	Renderer::RendererType Renderer::GetType() { return s_RendererType; }

	Ref<Renderer> Renderer::s_Renderer = nullptr;

	void Renderer::Create(GraphicsApi api, Renderer::RendererType type)
	{
		s_Api = api;
		switch (type)
		{
		case Renderer::RendererType::Forward:
			s_Renderer = CreateRef<ForwardRenderer>();
			break;
		case Renderer::RendererType::Deferred:
			s_Renderer = CreateRef<DeferredRenderer>();
			break;
		}
	}
}