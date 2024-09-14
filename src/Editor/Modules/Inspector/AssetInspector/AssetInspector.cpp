#include "pch.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "AssetInspector.h"
#include "Core/Base.h"
#include "UI/DwarfUI.h"
#include <iostream>
#include <string>
#include <variant>
// #include "Core/Scene/SceneUtilities.h"
// #include "Editor/Modules/Inspector/PreviewRenderer.h"
// #include "Input/InputManager.h"

#ifdef _WIN32
// #include "Platform/Direct3D12/D3D12Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif
// #include <Core/Rendering/Shader Parameters/BooleanShaderParameter.h>
// #include <Core/Rendering/Shader Parameters/IntegerShaderParameter.h>
// #include <Core/Rendering/Shader Parameters/UnsignedIntegerShaderParameter.h>
// #include <Core/Rendering/Shader Parameters/FloatShaderParameter.h>
// #include <Core/Rendering/Shader Parameters/Tex2DShaderParameter.h>
// #include <Core/Rendering/Shader Parameters/Vec2ShaderParameter.h>
// #include <Core/Rendering/Shader Parameters/Vec3ShaderParameter.h>
// #include <Core/Rendering/Shader Parameters/Vec4ShaderParameter.h>

#define COMPONENT_PANEL_PADDING (8.0f)
#define ADD_BUTTON_WIDTH (40.0f)
#define UNIFORM_DELETE_BUTTON_WIDTH (80.0f)

namespace Dwarf
{
  AssetInspector::AssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<ISceneIO>         sceneIO,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IMaterialPreview> materialPreview,
    std::shared_ptr<IModelPreview>    modelPreview,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorStats>     editorStats,
    std::shared_ptr<IAssetReimporter> assetReimporter)
    : m_LoadedScene(loadedScene)
    , m_SceneIO(sceneIO)
    , m_AssetDatabase(assetDatabase)
    , m_GraphicsApi(graphicsApi)
    , m_MaterialPreview(materialPreview)
    , m_ModelPreview(modelPreview)
    , m_InputManager(inputManager)
    , m_EditorStats(editorStats)
    , m_AssetReimporter(assetReimporter)
  {
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<UnknownAsset>(
    IAssetReference<UnknownAsset>& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Unknown");
  }

  struct RenderShaderParameterVisitor
  {
    std::shared_ptr<IAssetDatabase> AssetDatabase;
    std::string                     ParameterName;
    ParameterValue&                 Value;
    std::string                     ImGuiID;
    void
    operator()(bool parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::Checkbox(ImGuiID.c_str(), std::get_if<bool>(&Value));
    }

    void
    operator()(int parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputInt(ImGuiID.c_str(), std::get_if<int>(&Value));
      ImGui::PopItemWidth();
    }

    void
    operator()(unsigned int parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputScalar(
        ImGuiID.c_str(), ImGuiDataType_U32, std::get_if<unsigned int>(&Value));
      ImGui::PopItemWidth();
    }

    void
    operator()(float parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputFloat(ImGuiID.c_str(), std::get_if<float>(&Value));
      ImGui::PopItemWidth();
    }

    void
    operator()(Texture2DAssetValue parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);

      DwarfUI::AssetInput<TextureAsset>(
        AssetDatabase, std::get<Texture2DAssetValue>(Value), ImGuiID.c_str());
      ImGui::PopItemWidth();
    }

    void
    operator()(glm::vec2 parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputFloat2(ImGuiID.c_str(), &std::get<glm::vec2>(Value).x);
      ImGui::PopItemWidth();
    }

    void
    operator()(glm::vec3 parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputFloat3(ImGuiID.c_str(), &std::get<glm::vec3>(Value).x);
      ImGui::PopItemWidth();
    }

    void
    operator()(glm::vec4 parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::ColorEdit4(ImGuiID.c_str(),
                        &std::get<glm::vec4>(Value).x,
                        ImGuiColorEditFlags_None);
      ImGui::PopItemWidth();
    }
  };

  template<>
  void
  AssetInspector::RenderAssetInspector<MaterialAsset>(
    IAssetReference<MaterialAsset>& asset)
  {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    ImGui::BeginChild("##inspector_child",
                      ImGui::GetContentRegionAvail(),
                      false,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Dwarf Engine Material");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      // TODO: Fix: Crashes when not returned here
      // m_AssetDatabase->Reimport(asset->GetPath());
      m_AssetReimporter->QueueReimport(asset.GetPath());
      ImGui::EndChild();
      return;
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    auto separatorMin =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x +
               COMPONENT_PANEL_PADDING,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y +
               COMPONENT_PANEL_PADDING / 2.0f);
    auto separatorMax =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x -
               COMPONENT_PANEL_PADDING,
             separatorMin.y + 2);
    ImGui::GetWindowDrawList()->AddRectFilled(
      separatorMin, separatorMax, COL_BG_DIM);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    IMaterial& mat = asset.GetAsset().GetMaterial();

    ImGui::Checkbox("Transparent", &mat.GetMaterialProperties().IsTransparent);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING);

    if (ImGui::CollapsingHeader("Shader"))
    {
      ImGui::Indent(15.0f);
      switch (m_GraphicsApi)
      {
        using enum GraphicsApi;
#ifdef _WIN32
        case D3D12: break;
        case Metal: break;
        case OpenGL:
          {
            auto shader = (OpenGLShader*)mat->GetShader().get();
            ImGui::TextWrapped("Vertex Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<VertexShaderAsset>(
              m_AssetDatabase,
              shader->GetVertexShaderAsset(),
              "##vertexShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs().m_VertexShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Vertex Shader Error Log##vert"))
              {
                ImGui::TextWrapped(
                  "%s", shader->GetShaderLogs().m_VertexShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Tessellation Control Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<TessellationControlShaderAsset>(
              m_AssetDatabase,
              shader->GetTessellationControlShaderAsset(),
              "##tessellationControlShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs().m_TessellationControlShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader(
                    "Tessellation Control Shader Error Log##tesc"))
              {
                ImGui::TextWrapped("%s",
                                   shader->GetShaderLogs()
                                     .m_TessellationControlShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Tessellation Evaluation Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<TessellationEvaluationShaderAsset>(
              m_AssetDatabase,
              shader->GetTessellationEvaluationShaderAsset(),
              "##tessellationEvaluationShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs()
                   .m_TessellationEvaluationShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader(
                    "Tessellation Evaluation Shader Error Log##tese"))
              {
                ImGui::TextWrapped(
                  "%s",
                  shader->GetShaderLogs()
                    .m_TessellationEvaluationShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Geometry Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<GeometryShaderAsset>(
              m_AssetDatabase,
              shader->GetGeometryShaderAsset(),
              "##geometryShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs().m_GeometryShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Geometry Shader Error Log##geom"))
              {
                ImGui::TextWrapped(
                  "%s", shader->GetShaderLogs().m_GeometryShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Fragment Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<FragmentShaderAsset>(
              m_AssetDatabase,
              shader->GetFragmentShaderAsset(),
              "##fragmentShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs().m_FragmentShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Fragment Shader Error Log##frag"))
              {
                ImGui::TextWrapped(
                  "%s", shader->GetShaderLogs().m_FragmentShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }
            break;
          }
        case Vulkan: break;
#elif __linux__
        case D3D12: break;
        case Metal: break;
        case OpenGL:
          {
            auto shader = (OpenGLShader*)mat.GetShader().get();
            ImGui::TextWrapped("Vertex Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<VertexShaderAsset>(
              m_AssetDatabase,
              shader->GetVertexShaderAsset(),
              "##vertexShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs().m_VertexShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Vertex Shader Error Log##vert"))
              {
                ImGui::TextWrapped(
                  "%s", shader->GetShaderLogs().m_VertexShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Tessellation Control Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<TessellationControlShaderAsset>(
              m_AssetDatabase,
              shader->GetTessellationControlShaderAsset(),
              "##tessellationControlShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs().m_TessellationControlShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader(
                    "Tessellation Control Shader Error Log##tesc"))
              {
                ImGui::TextWrapped("%s",
                                   shader->GetShaderLogs()
                                     .m_TessellationControlShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Tessellation Evaluation Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<TessellationEvaluationShaderAsset>(
              m_AssetDatabase,
              shader->GetTessellationEvaluationShaderAsset(),
              "##tessellationEvaluationShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs()
                   .m_TessellationEvaluationShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader(
                    "Tessellation Evaluation Shader Error Log##tese"))
              {
                ImGui::TextWrapped(
                  "%s",
                  shader->GetShaderLogs()
                    .m_TessellationEvaluationShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Geometry Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<GeometryShaderAsset>(
              m_AssetDatabase,
              shader->GetGeometryShaderAsset(),
              "##geometryShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs().m_GeometryShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Geometry Shader Error Log##geom"))
              {
                ImGui::TextWrapped(
                  "%s", shader->GetShaderLogs().m_GeometryShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Fragment Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<FragmentShaderAsset>(
              m_AssetDatabase,
              shader->GetFragmentShaderAsset(),
              "##fragmentShader");
            ImGui::PopItemWidth();

            if (!shader->GetShaderLogs().m_FragmentShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Fragment Shader Error Log##frag"))
              {
                ImGui::TextWrapped(
                  "%s", shader->GetShaderLogs().m_FragmentShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }
            break;
          }
        case Vulkan: break;
#elif __APPLE__
        case D3D12: break;
        case Metal: break;
        case OpenGL: break;
        case Vulkan: break;
#endif
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

      if (ImGui::Button("Compile", ImVec2(60, 25)))
      {
        mat.GetShader()->Compile();
      }

      ImGui::SameLine();

      ImGui::TextWrapped(mat.GetShader()->IsCompiled() ? "Successfully Compiled"
                                                       : "Couldn't compile");
      ImGui::Unindent(15.0f);
    }

    if (ImGui::CollapsingHeader("Parameters"))
    {
      ImGui::Indent(15.0f);
      // Listing all current parameters (Excluding reserved uniform names by the
      // engine)
      int n = 0;
      for (auto paramIdentifier :
           mat.GetShaderParameters()->GetParameterIdentifiers())
      {
        if (std::ranges::find(std::begin(reservedParameterNames),
                              std::end(reservedParameterNames),
                              paramIdentifier) ==
            std::end(reservedParameterNames))
        {
          auto parameter =
            &mat.GetShaderParameters()->GetParameter(paramIdentifier);
          auto derefParam = *parameter;
          if (parameter)
          {
            std::visit(
              RenderShaderParameterVisitor{
                m_AssetDatabase,
                paramIdentifier,
                *parameter,
                std::format("##boolean{}", std::to_string(n++)) },
              derefParam);
            // Parameter UI specific to parameter type
            // switch (parameter->index())
            // {
            //   case:
            //     {
            //       std::shared_ptr<BooleanShaderParameter> parameter =
            //         std::dynamic_pointer_cast<BooleanShaderParameter>(
            //           i->second);
            //       ImGui::TextWrapped("%s", i->first.c_str());
            //       ImGui::SameLine();
            //       ImGui::Checkbox(
            //         std::format("##boolean{}", std::to_string(n)).c_str(),
            //         &(parameter->m_Value));
            //       n++;
            //     }
            //     break;
            //   case INTEGER:
            //     {
            //       std::shared_ptr<IntegerShaderParameter> parameter =
            //         std::dynamic_pointer_cast<IntegerShaderParameter>(
            //           i->second);
            //       ImGui::TextWrapped("%s", i->first.c_str());
            //       ImGui::SameLine();
            //       ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
            //                            UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
            //       ImGui::InputInt(
            //         std::format("##integer{}", std::to_string(n)).c_str(),
            //         &(parameter->m_Value));
            //       n++;
            //       ImGui::PopItemWidth();
            //     }
            //     break;
            //   case UNSIGNED_INTEGER:
            //     {
            //       std::shared_ptr<UnsignedIntegerShaderParameter> parameter =
            //         std::dynamic_pointer_cast<UnsignedIntegerShaderParameter>(
            //           i->second);
            //       ImGui::TextWrapped("%s", i->first.c_str());
            //       ImGui::SameLine();
            //       ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
            //                            UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
            //       ImGui::InputScalar(
            //         std::format("##unsignedInteger{}", std::to_string(n))
            //           .c_str(),
            //         ImGuiDataType_U32,
            //         &(parameter->m_Value));
            //       n++;
            //       ImGui::PopItemWidth();
            //     }
            //     break;
            //   case FLOAT:
            //     {
            //       std::shared_ptr<FloatShaderParameter> parameter =
            //         std::dynamic_pointer_cast<FloatShaderParameter>(i->second);
            //       ImGui::TextWrapped("%s", i->first.c_str());
            //       ImGui::SameLine();
            //       ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
            //                            UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
            //       ImGui::InputFloat(
            //         std::format("##float{}", std::to_string(n)).c_str(),
            //         &(parameter->m_Value));
            //       n++;
            //       ImGui::PopItemWidth();
            //     }
            //     break;
            //   case TEX2D:
            //     {
            //       std::shared_ptr<Tex2DShaderParameter> parameter =
            //         std::dynamic_pointer_cast<Tex2DShaderParameter>(i->second);
            //       ImGui::TextWrapped("%s", i->first.c_str());
            //       ImGui::SameLine();
            //       ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
            //                            UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
            //       DwarfUI::AssetInput<TextureAsset>(
            //         parameter->m_Value,
            //         std::format("##textureAsset{}",
            //         std::to_string(n)).c_str());
            //       n++;
            //       ImGui::PopItemWidth();
            //     }
            //     break;
            //   case VEC2:
            //     {
            //       std::shared_ptr<Vec2ShaderParameter> parameter =
            //         std::dynamic_pointer_cast<Vec2ShaderParameter>(i->second);
            //       ImGui::TextWrapped("%s", i->first.c_str());
            //       ImGui::SameLine();
            //       ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
            //                            UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
            //       ImGui::InputFloat2(
            //         std::format("##vec2{}", std::to_string(n)).c_str(),
            //         &(parameter->m_Value.x));
            //       n++;
            //       ImGui::PopItemWidth();
            //     }
            //     break;
            //   case VEC3:
            //     {
            //       std::shared_ptr<Vec3ShaderParameter> parameter =
            //         std::dynamic_pointer_cast<Vec3ShaderParameter>(i->second);
            //       ImGui::TextWrapped("%s", i->first.c_str());
            //       ImGui::SameLine();
            //       ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
            //                            UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
            //       ImGui::InputFloat3(
            //         std::format("##vec3{}", std::to_string(n)).c_str(),
            //         &(parameter->m_Value.x));
            //       n++;
            //       ImGui::PopItemWidth();
            //     }
            //     break;
            //   case VEC4:
            //     {
            //       std::shared_ptr<Vec4ShaderParameter> parameter =
            //         std::dynamic_pointer_cast<Vec4ShaderParameter>(i->second);
            //       ImGui::TextWrapped("%s", i->first.c_str());
            //       ImGui::SameLine();
            //       ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
            //                            UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
            //       ImGui::ColorEdit4(
            //         std::format("##vec4{}", std::to_string(n)).c_str(),
            //         &(parameter->m_Value.x),
            //         ImGuiColorEditFlags_None);
            //       n++;
            //       ImGui::PopItemWidth();
            //     }
            //     break;
            // }
          }

          // Delete button for parameter
          ImGui::SameLine();
          ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                               ImGui::GetContentRegionAvail().x -
                               UNIFORM_DELETE_BUTTON_WIDTH);
          if (ImGui::Button(
                std::format("Delete##{}", std::to_string(n)).c_str(),
                ImVec2(UNIFORM_DELETE_BUTTON_WIDTH - 15.0f, 0)))
          {
            // i = mat->m_Parameters.erase(i)--;
            mat.GetShaderParameters()->RemoveParameter(paramIdentifier);
          }
          else
          {
            n++;
          }
        }
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

      // Adding new parameter
      static char paramName[128] = "";
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           (100.0f + UNIFORM_DELETE_BUTTON_WIDTH + 15.0f));
      ImGui::InputTextWithHint("##paramName",
                               "Name",
                               paramName,
                               IM_ARRAYSIZE(paramName),
                               ImGuiInputTextFlags_CharsNoBlank);
      ImGui::PopItemWidth();
      ImGui::SameLine();
      static int selectedParameterType = 0;

      ImGui::PushItemWidth(100.0f);
      if (ImGui::BeginCombo("##paramType",
                            parameterTypeNames[selectedParameterType].c_str()))
      {
        for (int i = 0; i < 7; i++)
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
                        ImVec2(UNIFORM_DELETE_BUTTON_WIDTH - 15.0f, 0)) &&
          (std::strlen(paramName) > 0) &&
          !mat.GetShaderParameters()->HasParameter(paramName))
      {
        switch ((ShaderParameterType)selectedParameterType)
        {
          using enum ShaderParameterType;
          case BOOLEAN:
            mat.GetShaderParameters()->SetParameter(paramName, false);
            break;
          case INTEGER:
            mat.GetShaderParameters()->SetParameter(paramName, 0);
            break;
          case UNSIGNED_INTEGER:
            mat.GetShaderParameters()->SetParameter(paramName, 0u);
            break;
          case FLOAT:
            mat.GetShaderParameters()->SetParameter(paramName, 0.0f);
            break;
          case TEX2D:
            mat.GetShaderParameters()->SetParameter(
              paramName, Texture2DAssetValue(nullptr));
            break;
          case VEC2:
            mat.GetShaderParameters()->SetParameter(paramName,
                                                    glm::vec2(0.0f, 0.0f));
            break;
          case VEC3:
            mat.GetShaderParameters()->SetParameter(
              paramName, glm::vec3(0.0f, 0.0f, 0.0f));
            break;
          case VEC4:
            mat.GetShaderParameters()->SetParameter(
              paramName, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            break;
        }
        paramName[0] = '\0';
      }

      ImGui::Unindent(15.0f);
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Save changes",
                      ImVec2(ImGui::GetContentRegionAvail().x / 2.0f, 50)))
    {
      // MaterialSerializer::Serialize(*mat, asset->GetPath());
      ImGui::EndChild();
      return;
    }

    ImGui::SameLine();

    if (ImGui::Button("Generate parameters",
                      ImVec2(ImGui::GetContentRegionAvail().x, 50)))
    {
      mat.GenerateShaderParameters();
      mat.GetShader()->Compile();
    }

    ImGui::Text("Preview:");

    // PreviewRenderer::Resize(
    //   { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x
    //   });
    // PreviewRenderer::RenderMaterialPreview(asset);
    m_MaterialPreview->Resize(
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x });
    m_MaterialPreview->RenderMaterialPreview(asset);

    ImVec2 minRect = ImGui::GetCursorScreenPos();
    ImVec2 maxRect(
      ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
      ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x);

    static bool isRotating = false;
    if (m_InputManager->GetMouseButtonDown(Dwarf::MOUSE_BUTTON::LEFT) &&
        ImGui::IsMouseHoveringRect(
          { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y },
          { ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
            ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x }))
    {
      isRotating = true;
    }

    if (isRotating &&
        m_InputManager->GetMouseButtonUp(Dwarf::MOUSE_BUTTON::LEFT))
    {
      isRotating = false;
    }

    if (isRotating)
    {
      // PreviewRenderer::UpdateRotation(InputManager::GetDeltaMousePos());
      m_MaterialPreview->UpdateRotation(m_InputManager->GetMouseDelta());
    }

    draw_list->AddImage(m_MaterialPreview->GetTextureId(),
                        minRect,
                        maxRect,
                        ImVec2(0, 1),
                        ImVec2(1, 0));

    float endY = maxRect.y;
    ImGui::EndChild();

    draw_list->ChannelsSetCurrent(0);
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             endY + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0f);
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 *
    // COMPONENT_PANEL_PADDING);
    draw_list->ChannelsMerge();
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<SceneAsset>(
    IAssetReference<SceneAsset>& asset)
  {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    ImGui::BeginChild("##inspector_child",
                      ImGui::GetContentRegionAvail(),
                      false,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Dwarf Engine Scene");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      // m_AssetDatabase->Reimport(asset->GetPath());
      m_AssetReimporter->QueueReimport(asset.GetPath());
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    auto separatorMin =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x +
               COMPONENT_PANEL_PADDING,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y +
               COMPONENT_PANEL_PADDING / 2.0f);
    auto separatorMax =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x -
               COMPONENT_PANEL_PADDING,
             separatorMin.y + 2);
    ImGui::GetWindowDrawList()->AddRectFilled(
      separatorMin, separatorMax, COL_BG_DIM);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    if (ImGui::Button("Load scene") && FileHandler::FileExists(asset.GetPath()))
    {
      std::unique_ptr<IScene> loadedScene = m_SceneIO->LoadScene(asset);
      m_LoadedScene->SetScene(loadedScene);
    }

    draw_list->ChannelsSetCurrent(0);

    float endY = ImGui::GetItemRectMax().y;
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             endY + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
    draw_list->ChannelsMerge();
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<ModelAsset>(
    IAssetReference<ModelAsset>& asset)
  {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    ImGui::BeginChild("##inspector_child",
                      ImGui::GetContentRegionAvail(),
                      false,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Model");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      // m_AssetDatabase->Reimport(asset->GetPath());
      m_AssetReimporter->QueueReimport(asset.GetPath());
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    auto separatorMin =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x +
               COMPONENT_PANEL_PADDING,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y +
               COMPONENT_PANEL_PADDING / 2.0f);
    auto separatorMax =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x -
               COMPONENT_PANEL_PADDING,
             separatorMin.y + 2);
    ImGui::GetWindowDrawList()->AddRectFilled(
      separatorMin, separatorMax, COL_BG_DIM);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    ImGui::TextWrapped("TODO here will be settings for model import");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    ImGui::Text("Preview:");

    // PreviewRenderer::Resize(
    //   { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x
    //   });
    // PreviewRenderer::RenderModelPreview(asset);
    m_ModelPreview->Resize(
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x });
    m_ModelPreview->RenderModelPreview(asset);

    ImVec2 minRect = ImGui::GetCursorScreenPos();
    ImVec2 maxRect(
      ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
      ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x);

    static bool isRotating = false;
    if (ImGui::IsMouseHoveringRect(
          { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y },
          { ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
            ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x }))
    {
      if (m_InputManager->GetMouseButtonDown(Dwarf::MOUSE_BUTTON::LEFT))
      {
        isRotating = true;
      }

      m_ModelPreview->SetScrollDistance(
        std::max(0.0f,
                 std::min(1.0f,
                          m_ModelPreview->GetScrollDistance() +
                            m_InputManager->GetMouseScrollDelta().y *
                              (float)m_EditorStats->GetDeltaTime() * 1.7f)));
    }

    if (isRotating &&
        m_InputManager->GetMouseButtonUp(Dwarf::MOUSE_BUTTON::LEFT))
    {
      isRotating = false;
    }

    if (isRotating)
    {
      m_ModelPreview->UpdateRotation(m_InputManager->GetMouseDelta());
    }

    draw_list->AddImage(m_ModelPreview->GetTextureId(),
                        minRect,
                        maxRect,
                        ImVec2(0, 1),
                        ImVec2(1, 0));

    draw_list->ChannelsSetCurrent(0);

    float endY = maxRect.y;
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             endY + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0f);
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 *
    // COMPONENT_PANEL_PADDING);
    draw_list->ChannelsMerge();
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<TextureAsset>(
    IAssetReference<TextureAsset>& asset)
  {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    ImGui::BeginChild("##inspector_child",
                      ImGui::GetContentRegionAvail(),
                      false,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Texture");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      // m_AssetDatabase->Reimport(asset->GetPath());
      m_AssetReimporter->QueueReimport(asset.GetPath());
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    auto separatorMin =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x +
               COMPONENT_PANEL_PADDING,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y +
               COMPONENT_PANEL_PADDING / 2.0f);
    auto separatorMax =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x -
               COMPONENT_PANEL_PADDING,
             separatorMin.y + 2);
    ImGui::GetWindowDrawList()->AddRectFilled(
      separatorMin, separatorMax, COL_BG_DIM);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    float width = ImGui::GetContentRegionAvail().x;
    ImGui::TextWrapped("Preview:");

    auto texID = (ImTextureID)(asset.GetAsset().GetTexture().GetTextureID());
    // std::cout << "Texture ID: " << texID << std::endl;
    ImGui::Image(texID, ImVec2(width, width));

    draw_list->ChannelsSetCurrent(0);

    float endY = ImGui::GetItemRectMax().y;
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             endY + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
    draw_list->ChannelsMerge();
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<VertexShaderAsset>(
    IAssetReference<VertexShaderAsset>& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Vertex shader");
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<TessellationControlShaderAsset>(
    IAssetReference<TessellationControlShaderAsset>& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Tesselation control shader");
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<TessellationEvaluationShaderAsset>(
    IAssetReference<TessellationEvaluationShaderAsset>& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Tesselation evaluation shader");
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<GeometryShaderAsset>(
    IAssetReference<GeometryShaderAsset>& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Geometry shader");
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<FragmentShaderAsset>(
    IAssetReference<FragmentShaderAsset>& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Fragment shader");
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<ComputeShaderAsset>(
    IAssetReference<ComputeShaderAsset>& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Compute shader");
  }

  template<>
  void
  AssetInspector::RenderAssetInspector<HlslShaderAsset>(
    IAssetReference<HlslShaderAsset>& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Hlsl shader");
  }

  void
  AssetInspector::Render(const std::filesystem::path& assetPath)
  {
    if (m_AssetDatabase->Exists(assetPath))
    {
      using enum ASSET_TYPE;
      switch (m_AssetDatabase->GetAssetType(assetPath.extension().string()))
      {
        using enum ASSET_TYPE;
        case MODEL:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<ModelAsset>(assetPath));
          break;
        case TEXTURE:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<TextureAsset>(assetPath));
          break;
        case SCENE:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<SceneAsset>(assetPath));
          break;
        case MATERIAL:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<MaterialAsset>(assetPath));
          break;
        case VERTEX_SHADER:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<VertexShaderAsset>(assetPath));
          break;
        case TESC_SHADER:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<TessellationControlShaderAsset>(
              assetPath));
          break;
        case TESE_SHADER:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<TessellationEvaluationShaderAsset>(
              assetPath));
          break;
        case GEOMETRY_SHADER:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<GeometryShaderAsset>(assetPath));
          break;
        case FRAGMENT_SHADER:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<FragmentShaderAsset>(assetPath));
          break;
        case HLSL_SHADER:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<HlslShaderAsset>(assetPath));
          break;
        case COMPUTE_SHADER:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<ComputeShaderAsset>(assetPath));
          break;
        case UNKNOWN:
          RenderAssetInspector(
            *m_AssetDatabase->Retrieve<UnknownAsset>(assetPath));
          break;
        default: break;
      }
    }
    else
    {
      ImGui::TextWrapped("Asset not found.");
    }
  }
}