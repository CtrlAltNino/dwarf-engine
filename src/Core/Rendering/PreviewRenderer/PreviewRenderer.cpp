#include "pch.h"

#include "PreviewRenderer.h"

namespace Dwarf
{
  PreviewRenderer::PreviewRenderer(
    std::shared_ptr<IFramebufferFactory>        framebufferFactory,
    std::shared_ptr<ICamera>                    camera,
    const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
    std::shared_ptr<IEditorStats>               editorStats)
    : mFramebufferFactory(std::move(framebufferFactory))
    , mCamera(std::move(camera))
    , mRendererApi(rendererApiFactory->Create())
    , mEditorStats(std::move(editorStats))
  {
  }

  void
  PreviewRenderer::Resize(glm::ivec2 size)
  {
    if ((mRenderFramebuffer->GetSpecification().Width != size.x) &&
        (mRenderFramebuffer->GetSpecification().Height != size.y))
    {
      mCamera->GetProperties().AspectRatio = (float)size.y / (float)size.x;
      mRenderFramebuffer->Resize(size.x, size.y);
      mPreviewFramebuffer->Resize(size.x, size.y);
    }
  }

  void
  PreviewRenderer::UpdateRotation(glm::vec2 deltaMousePos)
  {
    mProperties.ModelRotationTarget = {
      std::max(
        -90.0F,
        std::min(90.0F,
                 mProperties.ModelRotationTarget.x + (deltaMousePos.y * 0.3F))),
      fmodf(mProperties.ModelRotationTarget.y + (deltaMousePos.x * 0.3F),
            360.0F),
      0
    };
  }

  auto
  PreviewRenderer::GetScrollDistance() const -> float
  {
    return mProperties.Distance;
  }

  void
  PreviewRenderer::SetScrollDistance(float distance)
  {
    mProperties.Distance = distance;
  }

  auto
  PreviewRenderer::GetTextureId() const -> ImTextureID
  {
    ImTextureID textureId = 0;

    if (mPreviewFramebuffer->GetColorAttachment(0))
    {
      textureId = (ImTextureID)mPreviewFramebuffer->GetColorAttachment(0)
                    .value()
                    .get()
                    .GetTextureID();
    }
    return textureId;
  }

  auto
  PreviewRenderer::EaseInOutQuad(float t) const -> float
  {
    return t * t * (3 - 2 * t);
  }

  auto
  PreviewRenderer::InterpolateVectors(const glm::vec3& currentVector,
                                      const glm::vec3& targetVector,
                                      float            speed) const -> glm::vec3
  {
    // Ensure speed is in the range (0, 1] for proper interpolation
    speed = std::max(0.001F, std::min(speed, 1.0F));

    // Calculate the interpolation factor using the easing function
    float t = EaseInOutQuad(speed);

    // Interpolate each component of the vectors
    float x = currentVector.x + (t * (targetVector.x - currentVector.x));
    float y = currentVector.y + (t * (targetVector.y - currentVector.y));
    float z = currentVector.z + (t * (targetVector.z - currentVector.z));

    return { x, y, z };
  }
}