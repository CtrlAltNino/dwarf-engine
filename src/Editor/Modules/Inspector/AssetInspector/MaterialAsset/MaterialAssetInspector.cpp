#include "MaterialAssetInspector.h"
#include <imgui.h>
#include <stdexcept>
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "UI/DwarfUI.h"

namespace Dwarf
{
  MaterialAssetInspector::MaterialAssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetReimporter> assetReimporter,
    std::shared_ptr<IMaterialPreview> materialPreview,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IMaterialIO>      materialIO)
    : m_GraphicsApi(graphicsApi)
    , m_AssetDatabase(assetDatabase)
    , m_AssetReimporter(assetReimporter)
    , m_MaterialPreview(materialPreview)
    , m_InputManager(inputManager)
    , m_MaterialIO(materialIO)
  {
    m_MaterialPreview->SetMeshType(MaterialPreviewMeshType::Sphere);
  }

  struct RenderShaderParameterVisitor
  {
    std::shared_ptr<IAssetDatabase> AssetDatabase;
    std::string                     ParameterName;
    ParameterValue&                 Value;
    std::string                     ImGuiID;
    void
    operator()(bool& parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::Checkbox(ImGuiID.c_str(), std::get_if<bool>(&Value));
    }

    void
    operator()(int& parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputInt(ImGuiID.c_str(), std::get_if<int>(&Value));
      ImGui::PopItemWidth();
    }

    void
    operator()(unsigned int& parameter)
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
    operator()(float& parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputFloat(ImGuiID.c_str(), std::get_if<float>(&Value));
      ImGui::PopItemWidth();
    }

    void
    operator()(Texture2DAssetValue& parameter)
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
    operator()(glm::vec2& parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputFloat2(ImGuiID.c_str(), &std::get<glm::vec2>(Value).x);
      ImGui::PopItemWidth();
    }

    void
    operator()(glm::vec3& parameter)
    {
      ImGui::TextWrapped("%s", ParameterName.c_str());
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           UNIFORM_DELETE_BUTTON_WIDTH - 8.0f);
      ImGui::InputFloat3(ImGuiID.c_str(), &std::get<glm::vec3>(Value).x);
      ImGui::PopItemWidth();
    }

    void
    operator()(glm::vec4& parameter)
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

  void
  MaterialAssetInspector::Render(IAssetReference& asset)
  {
    MaterialAsset& materialAsset = (MaterialAsset&)asset.GetAsset();
    IMaterial&     material = materialAsset.GetMaterial();

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

    ImGui::Checkbox("Transparent",
                    &material.GetMaterialProperties().IsTransparent);

    ImGui::Checkbox("Double Sided",
                    &material.GetMaterialProperties().IsDoubleSided);

    ImGui::Checkbox("Unlit", &material.GetMaterialProperties().IsUnlit);

    ImGui::Checkbox("Wireframe", &material.GetMaterialProperties().IsWireframe);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING);

    if (ImGui::CollapsingHeader("Shader"))
    {
      ImGui::Indent(15.0f);
      switch (m_GraphicsApi)
      {
        using enum GraphicsApi;
        case None: std::runtime_error("No graphics API selected"); break;
#ifdef _WIN32
        case D3D12: break;
        case Metal: break;
        case OpenGL:
          {
            IShader&      shader = material.GetShader();
            OpenGLShader& oglShader = dynamic_cast<OpenGLShader&>(shader);
            ImGui::TextWrapped("Vertex Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            std::cout << "VertexShaderAsset" << std::endl;
            DwarfUI::AssetInput<VertexShaderAsset>(
              m_AssetDatabase,
              oglShader.GetVertexShaderAsset(),
              "##vertexShader");
            ImGui::PopItemWidth();

            if (!oglShader.GetShaderLogs().m_VertexShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Vertex Shader Error Log##vert"))
              {
                ImGui::TextWrapped(
                  "%s", oglShader.GetShaderLogs().m_VertexShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Tessellation Control Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<TessellationControlShaderAsset>(
              m_AssetDatabase,
              oglShader.GetTessellationControlShaderAsset(),
              "##tessellationControlShader");
            ImGui::PopItemWidth();

            if (!oglShader.GetShaderLogs()
                   .m_TessellationControlShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader(
                    "Tessellation Control Shader Error Log##tesc"))
              {
                ImGui::TextWrapped("%s",
                                   oglShader.GetShaderLogs()
                                     .m_TessellationControlShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Tessellation Evaluation Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<TessellationEvaluationShaderAsset>(
              m_AssetDatabase,
              oglShader.GetTessellationEvaluationShaderAsset(),
              "##tessellationEvaluationShader");
            ImGui::PopItemWidth();

            if (!oglShader.GetShaderLogs()
                   .m_TessellationEvaluationShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader(
                    "Tessellation Evaluation Shader Error Log##tese"))
              {
                ImGui::TextWrapped(
                  "%s",
                  oglShader.GetShaderLogs()
                    .m_TessellationEvaluationShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Geometry Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<GeometryShaderAsset>(
              m_AssetDatabase,
              oglShader.GetGeometryShaderAsset(),
              "##geometryShader");
            ImGui::PopItemWidth();

            if (!oglShader.GetShaderLogs().m_GeometryShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Geometry Shader Error Log##geom"))
              {
                ImGui::TextWrapped(
                  "%s", oglShader.GetShaderLogs().m_GeometryShaderLog.c_str());
              }
              ImGui::PopItemWidth();
            }

            ImGui::TextWrapped("Fragment Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
            DwarfUI::AssetInput<FragmentShaderAsset>(
              m_AssetDatabase,
              oglShader.GetFragmentShaderAsset(),
              "##fragmentShader");
            ImGui::PopItemWidth();

            if (!oglShader.GetShaderLogs().m_FragmentShaderLog.empty())
            {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
              if (ImGui::CollapsingHeader("Fragment Shader Error Log##frag"))
              {
                ImGui::TextWrapped(
                  "%s", oglShader.GetShaderLogs().m_FragmentShaderLog.c_str());
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
            auto shader = (OpenGLShader*)material.GetShader().get();
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
        material.GetShader().Compile();
      }

      ImGui::SameLine();

      ImGui::TextWrapped(material.GetShader().IsCompiled()
                           ? "Successfully Compiled"
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
                m_AssetDatabase,
                paramIdentifier,
                parameter,
                std::format("##boolean{}", std::to_string(n++)) },
              parameter);
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
            //       std::shared_ptr<UnsignedIntegerShaderParameter> parameter
            //       =
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
            material.GetShaderParameters()->SetParameter(paramName, 0u);
            break;
          case FLOAT:
            material.GetShaderParameters()->SetParameter(paramName, 0.0f);
            break;
          case TEX2D:
            material.GetShaderParameters()->SetParameter(
              paramName, Texture2DAssetValue(nullptr));
            break;
          case VEC2:
            material.GetShaderParameters()->SetParameter(paramName,
                                                         glm::vec2(0.0f, 0.0f));
            break;
          case VEC3:
            material.GetShaderParameters()->SetParameter(
              paramName, glm::vec3(0.0f, 0.0f, 0.0f));
            break;
          case VEC4:
            material.GetShaderParameters()->SetParameter(
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
      m_MaterialIO->SaveMaterial(material, asset.GetPath());
      ImGui::EndChild();
      return;
    }

    ImGui::SameLine();

    if (ImGui::Button("Generate parameters",
                      ImVec2(ImGui::GetContentRegionAvail().x, 50)))
    {
      if (material.GetShader().IsCompiled())
      {
        material.GenerateShaderParameters();
      }
      // material.GetShader().Compile();
    }

    ImGui::Text("Preview:");

    m_MaterialPreview->Resize(
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x });
    m_MaterialPreview->RenderMaterialPreview(material);

    // Dropdown for mesh type
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::Text("Mesh Type:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    const char* meshTypeNames[] = { "Sphere", "Cube", "Plane" };
    if (ImGui::BeginCombo("##meshType",
                          meshTypeNames[(int)m_MaterialPreview->GetMeshType()]))
    {
      for (int i = 0; i < 3; i++)
      {
        if (ImGui::Selectable(meshTypeNames[i],
                              (int)m_MaterialPreview->GetMeshType() == i))
        {
          m_MaterialPreview->SetMeshType((MaterialPreviewMeshType)i);
        }
      }
      ImGui::EndCombo();
    }

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
      std::cout << "Start Rotating" << std::endl;
    }

    if (isRotating &&
        m_InputManager->GetMouseButtonUp(Dwarf::MOUSE_BUTTON::LEFT))
    {
      isRotating = false;
      std::cout << "Stopped rotating" << std::endl;
    }

    if (isRotating)
    {
      // PreviewRenderer::UpdateRotation(InputManager::GetDeltaMousePos());
      std::cout << "Rotating" << std::endl;
      m_MaterialPreview->UpdateRotation(m_InputManager->GetMouseDelta());
    }

    // std::cout << m_MaterialPreview->GetTextureId() << std::endl;

    draw_list->AddImage(m_MaterialPreview->GetTextureId(), minRect, maxRect);

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
}