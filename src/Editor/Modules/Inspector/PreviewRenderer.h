#pragma once
#include <imgui_internal.h>
#include "Core/Base.h"
#include "Core/Scene/Camera.h"
#include "Core/Rendering/Framebuffer.h"
#include "Core/Rendering/Forward/ForwardRenderer.h"
#include <Core/Asset/AssetComponents.h>
#include <Core/Asset/AssetComponents.h>
#include <Editor/EditorModel.h>

namespace Dwarf
{
  class PreviewRenderer
  {
  private:
    static std::shared_ptr<EditorModel> s_Model;
    static std::shared_ptr<Framebuffer> s_Framebuffer;

    static std::shared_ptr<Camera> s_Camera;

    static glm::vec3 s_ModelRotation;
    static glm::vec3 s_ModelRotationTarget;
    static glm::quat s_ModelRotationQuat;
    static float     s_RotationSpeed;
    static float     s_ScrollSpeed;
    static float     s_MaxDistance;
    static float     s_Distance;
    static float     s_DeltaTime;

  public:
    static void
    Init(std::shared_ptr<EditorModel> model);
    static void
    Resize(glm::ivec2 size);
    static void
    RenderModelPreview(std::shared_ptr<AssetReference<ModelAsset>> modelAsset);
    static void
    RenderMaterialPreview(
      std::shared_ptr<AssetReference<MaterialAsset>> materialAsset);
    static void
    FocusModel(std::shared_ptr<AssetReference<ModelAsset>> modelAsset);
    static void
    UpdateRotation(glm::vec2 deltaMousePos);
    static float
    GetScrollDistance();
    static void
    SetScrollDistance(float distance);
    static ImTextureID
    GetTextureId();
    static float
    EaseInOutQuad(float t);
    static glm::vec3
    InterpolateVectors(const glm::vec3& currentVector,
                       const glm::vec3& targetVector,
                       float            speed);
  };
}