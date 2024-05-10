#include "AssetInspectorRenderer.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Base.h"
#include "Core/UI/DwarfUI.h"
#include "Core/Scene/SceneUtilities.h"
#include <imgui_internal.h>
#include <math.h>
#include <format>
#include "Editor/Modules/Inspector/PreviewRenderer.h"
#include "Input/InputManager.h"

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
#include <Core/Rendering/Shader Parameters/BooleanShaderParameter.h>
#include <Core/Rendering/Shader Parameters/IntegerShaderParameter.h>
#include <Core/Rendering/Shader Parameters/UnsignedIntegerShaderParameter.h>
#include <Core/Rendering/Shader Parameters/FloatShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Tex2DShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec2ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec3ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec4ShaderParameter.h>

#include <algorithm>

#define COMPONENT_PANEL_PADDING (8.0f)
#define ADD_BUTTON_WIDTH (40.0f)
#define UNIFORM_DELETE_BUTTON_WIDTH (80.0f)

namespace Dwarf
{
  std::shared_ptr<EditorModel> AssetInspectorRenderer::s_Model = nullptr;

  void
  AssetInspectorRenderer::Init(std::shared_ptr<EditorModel> model)
  {
    AssetInspectorRenderer::s_Model = model;
    PreviewRenderer::Init(model);
  }
  template<>
  void
  AssetInspectorRenderer::RenderAssetInspector<AssetReference<UnknownAsset>>(
    std::shared_ptr<AssetReference<UnknownAsset>> asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Unknown");
  }

  template<>
  void
  AssetInspectorRenderer::RenderAssetInspector<AssetReference<MaterialAsset>>(
    std::shared_ptr<AssetReference<MaterialAsset>> asset)
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
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Dwarf Engine Material");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      // TODO: Fix: Crashes when not returned here
      AssetDatabase::Reimport(asset);
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

    std::shared_ptr<Material> mat = asset->GetAsset()->m_Material;

    ImGui::Checkbox("Transparent", &mat->m_Transparent);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING);

    if (ImGui::CollapsingHeader("Shader"))
    {
      ImGui::Indent(15.0f);
      switch (Renderer::GetAPI())
      {
#ifdef _WIN32
        case GraphicsApi::D3D12: break;
        case GraphicsApi::Metal: break;
        case GraphicsApi::OpenGL:
          {
            auto shader = (OpenGLShader*)mat->m_Shader.get();
            ImGui::TextWrapped("Vertex Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<VertexShaderAsset>(
              shader->GetShaderAssets().m_VertexShaderAsset, "##vertexShader");
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
            DwarfUI::AssetInput<TesselationControlShaderAsset>(
              shader->GetShaderAssets().m_TessellationControlShaderAsset,
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
            DwarfUI::AssetInput<TesselationEvaluationShaderAsset>(
              shader->GetShaderAssets().m_TessellationEvaluationShaderAsset,
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
              shader->GetShaderAssets().m_GeometryShaderAsset,
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
              shader->GetShaderAssets().m_FragmentShaderAsset,
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
        case GraphicsApi::Vulkan: break;
#elif __linux__
        case GraphicsApi::D3D12: break;
        case GraphicsApi::Metal: break;
        case GraphicsApi::OpenGL:
          {
            auto shader = (OpenGLShader*)mat->m_Shader.get();
            ImGui::TextWrapped("Vertex Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<VertexShaderAsset>(
              shader->GetShaderAssets().m_VertexShaderAsset, "##vertexShader");
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
            DwarfUI::AssetInput<TesselationControlShaderAsset>(
              shader->GetShaderAssets().m_TessellationControlShaderAsset,
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
            DwarfUI::AssetInput<TesselationEvaluationShaderAsset>(
              shader->GetShaderAssets().m_TessellationEvaluationShaderAsset,
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
              shader->GetShaderAssets().m_GeometryShaderAsset,
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
              shader->GetShaderAssets().m_FragmentShaderAsset,
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
        case GraphicsApi::Vulkan: break;
#elif __APPLE__
        case GraphicsApi::D3D12: break;
        case GraphicsApi::Metal: break;
        case GraphicsApi::OpenGL: break;
        case GraphicsApi::Vulkan: break;
#endif
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

      if (ImGui::Button("Compile", ImVec2(60, 25)))
      {
        mat->m_Shader->Compile();
      }

      ImGui::SameLine();

      ImGui::TextWrapped(mat->m_Shader->IsCompiled() ? "Successfully Compiled"
                                                     : "Couldn't compile");
      ImGui::Unindent(15.0f);
    }

    if (ImGui::CollapsingHeader("Parameters"))
    {
      ImGui::Indent(15.0f);
      // Listing all current parameters (Excluding reserved uniform names by the
      // engine)
      int n = 0;
      for (auto i = mat->m_Parameters.begin(); i != mat->m_Parameters.end();)
      {
        if (std::ranges::find(std::begin(reservedParameterNames),
                              std::end(reservedParameterNames),
                              i->first) == std::end(reservedParameterNames))
        {
          // Parameter UI specific to parameter type
          switch (i->second->GetType())
          {
            using enum ShaderParameterType;
            case BOOLEAN:
              {
                std::shared_ptr<BooleanShaderParameter> parameter =
                  std::dynamic_pointer_cast<BooleanShaderParameter>(i->second);
                ImGui::TextWrapped("%s", i->first.c_str());
                ImGui::SameLine();
                ImGui::Checkbox(
                  std::format("##boolean{}", std::to_string(n)).c_str(),
                  &(parameter->m_Value));
                n++;
              }
              break;
            case INTEGER:
              {
                std::shared_ptr<IntegerShaderParameter> parameter =
                  std::dynamic_pointer_cast<IntegerShaderParameter>(i->second);
                ImGui::TextWrapped("%s", i->first.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                                     UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
                ImGui::InputInt(
                  std::format("##integer{}", std::to_string(n)).c_str(),
                  &(parameter->m_Value));
                n++;
                ImGui::PopItemWidth();
              }
              break;
            case UNSIGNED_INTEGER:
              {
                std::shared_ptr<UnsignedIntegerShaderParameter> parameter =
                  std::dynamic_pointer_cast<UnsignedIntegerShaderParameter>(
                    i->second);
                ImGui::TextWrapped("%s", i->first.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                                     UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
                ImGui::InputScalar(
                  std::format("##unsignedInteger{}", std::to_string(n)).c_str(),
                  ImGuiDataType_U32,
                  &(parameter->m_Value));
                n++;
                ImGui::PopItemWidth();
              }
              break;
            case FLOAT:
              {
                std::shared_ptr<FloatShaderParameter> parameter =
                  std::dynamic_pointer_cast<FloatShaderParameter>(i->second);
                ImGui::TextWrapped("%s", i->first.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                                     UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
                ImGui::InputFloat(
                  std::format("##float{}", std::to_string(n)).c_str(),
                  &(parameter->m_Value));
                n++;
                ImGui::PopItemWidth();
              }
              break;
            case TEX2D:
              {
                std::shared_ptr<Tex2DShaderParameter> parameter =
                  std::dynamic_pointer_cast<Tex2DShaderParameter>(i->second);
                ImGui::TextWrapped("%s", i->first.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                                     UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
                DwarfUI::AssetInput<TextureAsset>(
                  parameter->m_Value,
                  std::format("##textureAsset{}", std::to_string(n)).c_str());
                n++;
                ImGui::PopItemWidth();
              }
              break;
            case VEC2:
              {
                std::shared_ptr<Vec2ShaderParameter> parameter =
                  std::dynamic_pointer_cast<Vec2ShaderParameter>(i->second);
                ImGui::TextWrapped("%s", i->first.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                                     UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
                ImGui::InputFloat2(
                  std::format("##vec2{}", std::to_string(n)).c_str(),
                  &(parameter->m_Value.x));
                n++;
                ImGui::PopItemWidth();
              }
              break;
            case VEC3:
              {
                std::shared_ptr<Vec3ShaderParameter> parameter =
                  std::dynamic_pointer_cast<Vec3ShaderParameter>(i->second);
                ImGui::TextWrapped("%s", i->first.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                                     UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
                ImGui::InputFloat3(
                  std::format("##vec3{}", std::to_string(n)).c_str(),
                  &(parameter->m_Value.x));
                n++;
                ImGui::PopItemWidth();
              }
              break;
            case VEC4:
              {
                std::shared_ptr<Vec4ShaderParameter> parameter =
                  std::dynamic_pointer_cast<Vec4ShaderParameter>(i->second);
                ImGui::TextWrapped("%s", i->first.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                                     UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
                ImGui::ColorEdit4(
                  std::format("##vec4{}", std::to_string(n)).c_str(),
                  &(parameter->m_Value.x),
                  ImGuiColorEditFlags_None);
                n++;
                ImGui::PopItemWidth();
              }
              break;
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
            i = mat->m_Parameters.erase(i)--;
          }
          else
          {
            n++;
          }
        }
        ++i;
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
          !mat->m_Parameters.contains(paramName))
      {
        switch ((ShaderParameterType)selectedParameterType)
        {
          using enum ShaderParameterType;
          case BOOLEAN: mat->SetParameter(paramName, false); break;
          case INTEGER: mat->SetParameter(paramName, 0); break;
          case UNSIGNED_INTEGER: mat->SetParameter(paramName, 0u); break;
          case FLOAT: mat->SetParameter(paramName, 0.0f); break;
          case TEX2D: mat->SetParameter(paramName, nullptr, TEX2D); break;
          case VEC2: mat->SetParameter(paramName, glm::vec2(0.0f, 0.0f)); break;
          case VEC3:
            mat->SetParameter(paramName, glm::vec3(0.0f, 0.0f, 0.0f));
            break;
          case VEC4:
            mat->SetParameter(paramName, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            break;
        }
        std::strcpy(paramName, "");
      }

      ImGui::Unindent(15.0f);
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Save changes",
                      ImVec2(ImGui::GetContentRegionAvail().x / 2.0f, 50)))
    {
      MaterialSerializer::Serialize(*mat, asset->GetPath());
      ImGui::EndChild();
      return;
    }

    ImGui::SameLine();

    if (ImGui::Button("Generate parameters",
                      ImVec2(ImGui::GetContentRegionAvail().x, 50)))
    {
      mat->GenerateShaderParameters();
    }

    ImGui::Text("Preview:");

    PreviewRenderer::Resize(
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x });
    PreviewRenderer::RenderMaterialPreview(asset);

    ImVec2 minRect = ImGui::GetCursorScreenPos();
    ImVec2 maxRect(
      ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
      ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x);

    static bool isRotating = false;
    if (InputManager::GetMouseDown(Dwarf::MOUSE_BUTTON::LEFT) &&
        ImGui::IsMouseHoveringRect(
          { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y },
          { ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
            ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x }))
    {
      isRotating = true;
    }

    if (isRotating && InputManager::GetMouseUp(Dwarf::MOUSE_BUTTON::LEFT))
    {
      isRotating = false;
    }

    if (isRotating)
    {
      PreviewRenderer::UpdateRotation(InputManager::GetDeltaMousePos());
    }

    draw_list->AddImage(PreviewRenderer::GetTextureId(),
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
  AssetInspectorRenderer::RenderAssetInspector<AssetReference<SceneAsset>>(
    std::shared_ptr<AssetReference<SceneAsset>> asset)
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
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Dwarf Engine Scene");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      AssetDatabase::Reimport(asset);
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

    if (ImGui::Button("Load scene") &&
        FileHandler::CheckIfFileExists(asset->GetPath()))
    {
      AssetInspectorRenderer::s_Model->SetScene(
        SceneUtilities::LoadScene(asset->GetPath()));
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
  AssetInspectorRenderer::RenderAssetInspector<AssetReference<ModelAsset>>(
    std::shared_ptr<AssetReference<ModelAsset>> asset)
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
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Model");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      AssetDatabase::Reimport(asset);
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

    PreviewRenderer::Resize(
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x });
    PreviewRenderer::RenderModelPreview(asset);

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
      if (InputManager::GetMouseDown(Dwarf::MOUSE_BUTTON::LEFT))
      {
        isRotating = true;
      }

      PreviewRenderer::SetScrollDistance(
        std::max(0.0f,
                 std::min(1.0f,
                          PreviewRenderer::GetScrollDistance() +
                            InputManager::GetDeltaScroll().y *
                              (float)s_Model->GetDeltaTime() * 1.7f)));
    }

    if (isRotating && InputManager::GetMouseUp(Dwarf::MOUSE_BUTTON::LEFT))
    {
      isRotating = false;
    }

    if (isRotating)
    {
      PreviewRenderer::UpdateRotation(InputManager::GetDeltaMousePos());
    }

    draw_list->AddImage(PreviewRenderer::GetTextureId(),
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
  AssetInspectorRenderer::RenderAssetInspector<AssetReference<TextureAsset>>(
    std::shared_ptr<AssetReference<TextureAsset>> asset)
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
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Texture");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      AssetDatabase::Reimport(asset);
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

    auto texID = (ImTextureID)asset->GetAsset()->m_Texture->GetTextureID();
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
  AssetInspectorRenderer::RenderAssetInspector<
    AssetReference<VertexShaderAsset>>(
    std::shared_ptr<AssetReference<VertexShaderAsset>> asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Vertex shader");
  }

  template<>
  void
  AssetInspectorRenderer::RenderAssetInspector<
    AssetReference<TesselationControlShaderAsset>>(
    std::shared_ptr<AssetReference<TesselationControlShaderAsset>> asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Tesselation control shader");
  }

  template<>
  void
  AssetInspectorRenderer::RenderAssetInspector<
    AssetReference<TesselationEvaluationShaderAsset>>(
    std::shared_ptr<AssetReference<TesselationEvaluationShaderAsset>> asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Tesselation evaluation shader");
  }

  template<>
  void
  AssetInspectorRenderer::RenderAssetInspector<
    AssetReference<GeometryShaderAsset>>(
    std::shared_ptr<AssetReference<GeometryShaderAsset>> asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Geometry shader");
  }

  template<>
  void
  AssetInspectorRenderer::RenderAssetInspector<
    AssetReference<FragmentShaderAsset>>(
    std::shared_ptr<AssetReference<FragmentShaderAsset>> asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Fragment shader");
  }

  template<>
  void
  AssetInspectorRenderer::RenderAssetInspector<
    AssetReference<ComputeShaderAsset>>(
    std::shared_ptr<AssetReference<ComputeShaderAsset>> asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Compute shader");
  }

  template<>
  void
  AssetInspectorRenderer::RenderAssetInspector<AssetReference<HlslShaderAsset>>(
    std::shared_ptr<AssetReference<HlslShaderAsset>> asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset->GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Hlsl shader");
  }
}