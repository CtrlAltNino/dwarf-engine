#include "PreviewRenderer.h"

namespace Dwarf
{
  PreviewRenderer::PreviewRenderer(
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ICamera>             camera,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IEditorStats>        editorStats)
    : m_FramebufferFactory(framebufferFactory)
    , m_Camera(camera)
    , m_RendererApi(rendererApiFactory->Create())
    , m_EditorStats(editorStats)
  {
  }

  void
  PreviewRenderer::Resize(glm::ivec2 size)
  {
    if ((m_RenderFramebuffer->GetSpecification().Width != size.x) &&
        (m_RenderFramebuffer->GetSpecification().Height != size.y))
    {
      m_Camera->GetProperties().AspectRatio = (float)size.y / (float)size.x;
      m_RenderFramebuffer->Resize(size.x, size.y);
      m_PreviewFramebuffer->Resize(size.x, size.y);
    }
  }

  void
  PreviewRenderer::UpdateRotation(glm::vec2 deltaMousePos)
  {
    m_Properties.ModelRotationTarget = {
      std::max(
        -90.0f,
        std::min(90.0f, m_Properties.ModelRotationTarget.x - deltaMousePos.y)),
      fmodf(m_Properties.ModelRotationTarget.y + deltaMousePos.x, 360.0f),
      0
    };
  }

  float
  PreviewRenderer::GetScrollDistance() const
  {
    return m_Properties.Distance;
  }

  void
  PreviewRenderer::SetScrollDistance(float distance)
  {
    m_Properties.Distance = distance;
  }

  ImTextureID
  PreviewRenderer::GetTextureId() const
  {
    ImTextureID textureId = 0;

    if (m_PreviewFramebuffer->GetColorAttachment(0))
    {
      textureId = (ImTextureID)m_PreviewFramebuffer->GetColorAttachment(0)
                    .value()
                    .get()
                    .GetTextureID();
    }
    return textureId;
  }

  float
  PreviewRenderer::EaseInOutQuad(float t) const
  {
    return t * t * (3 - 2 * t);
  }

  glm::vec3
  PreviewRenderer::InterpolateVectors(const glm::vec3& currentVector,
                                      const glm::vec3& targetVector,
                                      float            speed) const
  {
    // Ensure speed is in the range (0, 1] for proper interpolation
    speed = std::max(0.001f, std::min(speed, 1.0f));

    // Calculate the interpolation factor using the easing function
    float t = EaseInOutQuad(speed);

    // Interpolate each component of the vectors
    float x = currentVector.x + t * (targetVector.x - currentVector.x);
    float y = currentVector.y + t * (targetVector.y - currentVector.y);
    float z = currentVector.z + t * (targetVector.z - currentVector.z);

    return { x, y, z };
  }
}