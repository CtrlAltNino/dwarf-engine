#include "pch.h"

#include "Core/Asset/AssetTypes.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "MaterialAssetInspector.h"
#include "UI/DwarfUI.h"
#include <memory>

namespace Dwarf
{
  MaterialAssetInspector::MaterialAssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetReimporter> assetReimporter,
    std::shared_ptr<IMaterialPreview> materialPreview,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IShaderRegistry>  shaderRegistry,
    const std::shared_ptr<IShaderAssetSelectorFactory>&
      shaderAssetSelectorFactory)
    : mGraphicsApi(graphicsApi)
    , mAssetDatabase(std::move(assetDatabase))
    , mAssetReimporter(std::move(assetReimporter))
    , mMaterialPreview(std::move(materialPreview))
    , mInputManager(std::move(inputManager))
    , mMaterialIO(std::move(materialIO))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderAssetSelector(shaderAssetSelectorFactory->Create())
  {
    mMaterialPreview->SetMeshType(MaterialPreviewMeshType::Sphere);
  }

  struct RenderShaderParameterVisitor
  {
    std::shared_ptr<IAssetDatabase> AssetDatabase;
    std::string                     ParameterName;
    std::string                     ImGuiID;

    void
    operator()(bool& parameter) const
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::Checkbox(ImGuiID.c_str(), &parameter);
    }

    void
    operator()(int& parameter) const
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0F);
      ImGui::InputInt(ImGuiID.c_str(), &parameter);
      ImGui::PopItemWidth();
    }

    void
    operator()(uint32_t& parameter) const
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0F);
      ImGui::InputScalar(ImGuiID.c_str(), ImGuiDataType_U32, &parameter);
      ImGui::PopItemWidth();
    }

    void
    operator()(float& parameter) const
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0F);
      ImGui::InputFloat(ImGuiID.c_str(), &parameter);
      ImGui::PopItemWidth();
    }

    void
    operator()(Texture2DAssetValue& parameter) const
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0F);

      DwarfUI::AssetInput<TextureAsset>(
        AssetDatabase, parameter, ImGuiID.c_str());
      ImGui::PopItemWidth();
    }

    void
    operator()(glm::vec2& parameter) const
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0F);
      ImGui::InputFloat2(ImGuiID.c_str(), &parameter.x);
      ImGui::PopItemWidth();
    }

    void
    operator()(glm::vec3& parameter) const
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0F);
      ImGui::InputFloat3(ImGuiID.c_str(), &parameter.x);
      ImGui::PopItemWidth();
    }

    void
    operator()(glm::vec4& parameter) const
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0F);
      ImGui::ColorEdit4(
        ImGuiID.c_str(), &parameter.x, ImGuiColorEditFlags_None);
      ImGui::PopItemWidth();
    }
  };

  void
  MaterialAssetInspector::Render(IAssetReference& asset)
  {
    if (asset.GetType() != ASSET_TYPE::MATERIAL)
    {
      return;
    }

    if (mMaterialAssetMemory == nullptr ||
        mMaterialAssetMemory !=
          &dynamic_cast<MaterialAsset&>(asset.GetAsset()).GetMaterial())
    {
      mCurrentAsset = dynamic_cast<MaterialAsset&>(asset.GetAsset());
      mAssetPath = asset.GetPath();
      mMaterialAssetMemory = &mCurrentAsset->get().GetMaterial();
      mShaderAssetSelector->SetCurrentShader(
        mCurrentAsset->get().GetMaterial().GetShader(),
        mCurrentAsset->get().GetMaterial().GetShaderAssetSources());
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(2);
    drawList->ChannelsSetCurrent(1);

    RenderAssetInfo();

    ImGui::Spacing();

    RenderMaterialProperties();

    ImGui::Spacing();

    RenderMaterialShader();

    ImGui::Spacing();

    RenderMaterialButtons();

    float endY = RenderMaterialPreview();
  }

  void
  MaterialAssetInspector::RenderAssetInfo()
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("%s", mAssetPath.filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("%s", mAssetPath.string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("Dwarf Engine Material");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      // TODO: Fix: Crashes when not returned here
      mAssetReimporter->QueueReimport(mAssetPath);
      return;
    }
  }

  void
  MaterialAssetInspector::RenderMaterialProperties()
  {
    if (!mCurrentAsset.has_value())
    {
      return;
    }

    IMaterial& material = mCurrentAsset->get().GetMaterial();

    auto separatorMin =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x +
               COMPONENT_PANEL_PADDING,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y +
               (COMPONENT_PANEL_PADDING / 2.0F));
    auto separatorMax =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x -
               COMPONENT_PANEL_PADDING,
             separatorMin.y + 2);
    ImGui::GetWindowDrawList()->AddRectFilled(
      separatorMin, separatorMax, COL_BG_DIM);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    ImGui::Checkbox("Transparent",
                    &material.GetMaterialProperties().IsTransparent);

    ImGui::Checkbox("Double Sided",
                    &material.GetMaterialProperties().IsDoubleSided);

    ImGui::Checkbox("Unlit", &material.GetMaterialProperties().IsUnlit);

    ImGui::Checkbox("Wireframe", &material.GetMaterialProperties().IsWireframe);
  }

  void
  MaterialAssetInspector::RenderMaterialShader()
  {
    RenderShaderAssetDropdowns();
    RenderShaderParameters();
  }

  void
  MaterialAssetInspector::RenderShaderAssetDropdowns()
  {
    if (!mCurrentAsset.has_value())
    {
      return;
    }

    IMaterial& material = mCurrentAsset->get().GetMaterial();

    if (ImGui::CollapsingHeader("Shader"))
    {
      ImGui::Indent(15.0F);

      mShaderAssetSelector->Render();

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

      if (ImGui::Button("Compile", ImVec2(60, 25)))
      {
        // material.SetShader(mShaderRegistry->GetOrCreate(
        //   mShaderAssetSelector->GetCurrentSelection()));
        // mShaderAssetSelector->SetCurrentShader(material.GetShader());
        material.UpdateShader();
        if (!material.GetShader()->IsCompiled())
        {
          material.GetShader()->Compile();
        }
      }

      ImGui::SameLine();

      ImGui::TextWrapped(material.GetShader()->IsCompiled()
                           ? "Successfully Compiled"
                           : "Couldn't compile");
      ImGui::Unindent(15.0f);
    }
  }

  void
  MaterialAssetInspector::RenderShaderParameters()
  {
    if (!mCurrentAsset.has_value())
    {
      return;
    }

    IMaterial& material = mCurrentAsset->get().GetMaterial();

    if (ImGui::CollapsingHeader("Parameters"))
    {
      ImGui::Indent(15.0F);
      // Listing all current parameters (Excluding reserved uniform names by the
      // engine)
      int n = 0;
      for (const auto& paramIdentifier :
           material.GetShaderParameters()->GetParameterIdentifiers())
      {
        if (std::ranges::find(std::begin(reservedParameterNames),
                              std::end(reservedParameterNames),
                              paramIdentifier) ==
            std::end(reservedParameterNames))
        {
          if (material.GetShaderParameters()->HasParameter(paramIdentifier))
          {

            ParameterValue& parameter =
              material.GetShaderParameters()->GetParameter(paramIdentifier);
            std::visit(
              RenderShaderParameterVisitor{
                .AssetDatabase = mAssetDatabase,
                .ParameterName = paramIdentifier,
                .ImGuiID =
                  std::format("##{}{}", paramIdentifier, std::to_string(n++)) },
              parameter);
          }

          // Delete button for parameter
          ImGui::SameLine();
          ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                               ImGui::GetContentRegionAvail().x -
                               UNIFORM_DELETE_BUTTON_WIDTH);
          if (ImGui::Button(
                std::format("Delete##{}", std::to_string(n)).c_str(),
                ImVec2(UNIFORM_DELETE_BUTTON_WIDTH - 15.0F, 0)))
          {
            // i = mat->mParameters.erase(i)--;
            material.GetShaderParameters()->RemoveParameter(paramIdentifier);
          }
          else
          {
            n++;
          }
        }
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

      // Adding new parameter
      static std::string paramName;
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           (100.0f + UNIFORM_DELETE_BUTTON_WIDTH + 15.0f));
      ImGui::InputTextWithHint("##paramName",
                               "Name",
                               paramName.data(),
                               paramName.capacity() + 1,
                               ImGuiInputTextFlags_CharsNoBlank |
                                 ImGuiInputTextFlags_CallbackResize,
                               DwarfUI::InputTextCallback,
                               &paramName);
      ImGui::PopItemWidth();
      ImGui::SameLine();
      static int selectedParameterType = 0;

      ImGui::PushItemWidth(100.0f);
      if (ImGui::BeginCombo("##paramType",
                            parameterTypeNames[selectedParameterType].c_str()))
      {
        for (int i = 0; i < parameterTypeNames.size(); i++)
        {
          if (ImGui::Selectable(parameterTypeNames[i].c_str(),
                                i == selectedParameterType))
          {
            selectedParameterType = i;
          }
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      ImGui::SameLine();
      if (ImGui::Button("Add##addParam",
                        ImVec2(UNIFORM_DELETE_BUTTON_WIDTH - 15.0F, 0)) &&
          (paramName.length() > 0) &&
          !material.GetShaderParameters()->HasParameter(paramName))
      {
        switch ((ShaderParameterType)selectedParameterType)
        {
          using enum ShaderParameterType;
          case BOOLEAN:
            material.GetShaderParameters()->SetParameter(paramName, false);
            break;
          case INTEGER:
            material.GetShaderParameters()->SetParameter(paramName, 0);
            break;
          case UNSIGNED_INTEGER:
            material.GetShaderParameters()->SetParameter(paramName, 0U);
            break;
          case FLOAT:
            material.GetShaderParameters()->SetParameter(paramName, 0.0F);
            break;
          case TEX2D:
            material.GetShaderParameters()->SetParameter(
              paramName, Texture2DAssetValue(nullptr));
            break;
          case VEC2:
            material.GetShaderParameters()->SetParameter(paramName,
                                                         glm::vec2(0.0F, 0.0F));
            break;
          case VEC3:
            material.GetShaderParameters()->SetParameter(
              paramName, glm::vec3(0.0F, 0.0F, 0.0F));
            break;
          case VEC4:
            material.GetShaderParameters()->SetParameter(
              paramName, glm::vec4(0.0F, 0.0F, 0.0F, 0.0F));
            break;
        }
        paramName[0] = '\0';
      }

      ImGui::Unindent(15.0F);
    }
  }

  void
  MaterialAssetInspector::RenderMaterialButtons()
  {
    if (!mCurrentAsset.has_value())
    {
      return;
    }

    IMaterial& material = mCurrentAsset->get().GetMaterial();

    if (ImGui::Button("Save changes",
                      ImVec2(ImGui::GetContentRegionAvail().x / 2.0F, 50)))
    {
      mMaterialIO->SaveMaterial(material, mAssetPath);
      return;
    }

    ImGui::SameLine();

    if (ImGui::Button("Generate parameters",
                      ImVec2(ImGui::GetContentRegionAvail().x, 50)))
    {
      if (material.GetShader()->IsCompiled())
      {
        material.GenerateShaderParameters();
      }
    }
  }

  auto
  MaterialAssetInspector::RenderMaterialPreview() -> float
  {
    if (!mCurrentAsset.has_value())
    {
      return 0;
    }

    IMaterial& material = mCurrentAsset->get().GetMaterial();

    ImGui::Text("Preview:");

    mMaterialPreview->Resize(
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x });
    mMaterialPreview->RenderMaterialPreview(material);

    // Dropdown for mesh type
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                         (3 * COMPONENT_PANEL_PADDING));
    ImGui::Text("Mesh Type:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    const char* meshTypeNames[] = { "Sphere", "Cube", "Plane" };
    if (ImGui::BeginCombo("##meshType",
                          meshTypeNames[(int)mMaterialPreview->GetMeshType()]))
    {
      for (int i = 0; i < 3; i++)
      {
        if (ImGui::Selectable(meshTypeNames[i],
                              (int)mMaterialPreview->GetMeshType() == i))
        {
          mMaterialPreview->SetMeshType((MaterialPreviewMeshType)i);
        }
      }
      ImGui::EndCombo();
    }

    static bool isRotating = false;
    if (mInputManager->GetMouseButtonDown(Dwarf::MOUSE_BUTTON::LEFT) &&
        ImGui::IsMouseHoveringRect(
          { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y },
          { ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
            ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x }))
    {
      isRotating = true;
    }

    if (isRotating &&
        mInputManager->GetMouseButtonUp(Dwarf::MOUSE_BUTTON::LEFT))
    {
      isRotating = false;
    }

    if (isRotating)
    {
      mMaterialPreview->UpdateRotation(mInputManager->GetMouseDelta());
    }

    ImGui::Image(
      mMaterialPreview->GetTextureId(),
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x },
      ImVec2(0, 1),
      ImVec2(1, 0));

    return ImGui::GetItemRectMax().y;
  }
}