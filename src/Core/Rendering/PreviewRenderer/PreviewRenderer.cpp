#include "PreviewRenderer.h"

namespace Dwarf
{
  PreviewRenderer::PreviewRenderer(
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ICamera>             camera,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory)
    : m_FramebufferFactory(framebufferFactory)
    , m_Camera(camera)
    , m_RendererApi(rendererApiFactory->Create())
  {
  }

  void
  PreviewRenderer::Resize(glm::ivec2 size)
  {
    if ((m_Framebuffer->GetSpecification().Width != size.x) &&
        (m_Framebuffer->GetSpecification().Height != size.y))
    {
      m_Camera->GetProperties().AspectRatio = (float)size.y / (float)size.x;
      m_Framebuffer->Resize(size.x, size.y);
    }
  }

  void
  PreviewRenderer::UpdateRotation(glm::vec2 deltaMousePos)
  {
    m_Properties.ModelRotationTarget.x +=
      deltaMousePos.y * m_Properties.RotationSpeed;
    m_Properties.ModelRotationTarget.y +=
      deltaMousePos.x * m_Properties.RotationSpeed;

    m_Properties.ModelRotation.x = EaseInOutQuad(m_Properties.ModelRotation.x);
    m_Properties.ModelRotation.y = EaseInOutQuad(m_Properties.ModelRotation.y);

    m_Properties.ModelRotationQuat =
      glm::quat(glm::radians(m_Properties.ModelRotation));
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
    return (ImTextureID)m_Framebuffer->GetColorAttachment(0)->GetTextureID();
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