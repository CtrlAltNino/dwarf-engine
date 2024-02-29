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
        static Ref<EditorModel> s_Model;
        static Ref<Framebuffer> s_Framebuffer;

        static Ref<Camera> s_Camera;

        static glm::vec3 s_ModelRotation;
        static glm::quat s_ModelRotationQuat;
        static float s_MaxDistance;
        static float s_Distance;
        static float s_DeltaTime;

    public:
        static void Init(Ref<EditorModel> model);
        static void Resize(glm::ivec2 size);
        static void RenderModelPreview(Ref<AssetReference<ModelAsset>> modelAsset);
        static void RenderMaterialPreview(Ref<AssetReference<MaterialAsset>> materialAsset);
        static void FocusModel(Ref<AssetReference<ModelAsset>> modelAsset);
        static void UpdateRotation(glm::vec2 deltaMousePos);
        static ImTextureID GetTextureId();
    };
}