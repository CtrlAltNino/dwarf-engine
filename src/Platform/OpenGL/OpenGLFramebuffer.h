#pragma once

#include "Core/Rendering/Framebuffer.h"

namespace Dwarf
{

	class OpenGLFramebuffer : public Framebuffer
	{
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;

	public:
		OpenGLFramebuffer(const FramebufferSpecification &spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual const uint32_t *GetColorAttachmentRendererID(uint32_t index = 0) const override
		{
			// return m_ColorAttachments[index];
			if (index >= 0 && index < m_ColorAttachments.size())
			{
				// Return the address of the value at the specified index
				return &m_ColorAttachments[index];
			}
			else
			{
				// If the index is out of bounds, return nullptr or handle the error accordingly
				return nullptr;
			}
		}

		virtual const FramebufferSpecification &GetSpecification() const override { return m_Specification; }
	};

}