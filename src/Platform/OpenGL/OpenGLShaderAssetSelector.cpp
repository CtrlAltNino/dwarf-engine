#include "pch.h"

#include "Core/Asset/Database/AssetComponents.h"
#include "OpenGLShaderAssetSelector.h"
#include "OpenGLShaderAssetSourceContainer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "UI/DwarfUI.h"

namespace Dwarf
{
  OpenGLShaderAssetSelector::OpenGLShaderAssetSelector(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : mLogger(std::move(logger))
    , mAssetDatabase(std::move(assetDatabase))
  {
    mLogger->LogDebug(
      Log("OpenGLShaderAssetSelector created", "OpenGLShaderAssetSelector"));
  }

  OpenGLShaderAssetSelector::~OpenGLShaderAssetSelector()
  {
    mLogger->LogDebug(
      Log("OpenGLShaderAssetSelector destroyed", "OpenGLShaderAssetSelector"));
  }

  void
  OpenGLShaderAssetSelector::SetCurrentShader(
    std::shared_ptr<IShader> shader,
    std::unique_ptr<IShaderAssetSourceContainer>&
      selectedShaderAssetSourceContainer)
  {
    mSelectedShader = shader;
    mSelectedShaderAssetSourceContainer = selectedShaderAssetSourceContainer;
  }

  void
  OpenGLShaderAssetSelector::Render()
  {
    auto& oglShader = dynamic_cast<OpenGLShader&>(*mSelectedShader);
    auto& oglShaderAssets = dynamic_cast<OpenGLShaderAssetSourceContainer&>(
      *(mSelectedShaderAssetSourceContainer->get()));

    ImGui::TextWrapped("Vertex Shader");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
    DwarfUI::AssetInput<VertexShaderAsset>(
      mAssetDatabase, oglShaderAssets.GetVertexShaderAsset(), "##vertexShader");
    ImGui::PopItemWidth();

    if (!oglShader.GetShaderLogs().mVertexShaderLog.empty())
    {
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
      if (ImGui::CollapsingHeader("Vertex Shader Error Log##vert"))
      {
        ImGui::TextWrapped("%s",
                           oglShader.GetShaderLogs().mVertexShaderLog.c_str());
      }
      ImGui::PopItemWidth();
    }

    ImGui::TextWrapped("Tessellation Control Shader");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
    DwarfUI::AssetInput<TessellationControlShaderAsset>(
      mAssetDatabase,
      oglShaderAssets.GetTessellationControlShaderAsset(),
      "##tessellationControlShader");
    ImGui::PopItemWidth();

    if (!oglShader.GetShaderLogs().mTessellationControlShaderLog.empty())
    {
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
      if (ImGui::CollapsingHeader(
            "Tessellation Control Shader Error Log##tesc"))
      {
        ImGui::TextWrapped(
          "%s",
          oglShader.GetShaderLogs().mTessellationControlShaderLog.c_str());
      }
      ImGui::PopItemWidth();
    }

    ImGui::TextWrapped("Tessellation Evaluation Shader");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
    DwarfUI::AssetInput<TessellationEvaluationShaderAsset>(
      mAssetDatabase,
      oglShaderAssets.GetTessellationEvaluationShaderAsset(),
      "##tessellationEvaluationShader");
    ImGui::PopItemWidth();

    if (!oglShader.GetShaderLogs().mTessellationEvaluationShaderLog.empty())
    {
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
      if (ImGui::CollapsingHeader(
            "Tessellation Evaluation Shader Error Log##tese"))
      {
        ImGui::TextWrapped(
          "%s",
          oglShader.GetShaderLogs().mTessellationEvaluationShaderLog.c_str());
      }
      ImGui::PopItemWidth();
    }

    ImGui::TextWrapped("Geometry Shader");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
    DwarfUI::AssetInput<GeometryShaderAsset>(
      mAssetDatabase,
      oglShaderAssets.GetGeometryShaderAsset(),
      "##geometryShader");
    ImGui::PopItemWidth();

    if (!oglShader.GetShaderLogs().mGeometryShaderLog.empty())
    {
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
      if (ImGui::CollapsingHeader("Geometry Shader Error Log##geom"))
      {
        ImGui::TextWrapped(
          "%s", oglShader.GetShaderLogs().mGeometryShaderLog.c_str());
      }
      ImGui::PopItemWidth();
    }

    ImGui::TextWrapped("Fragment Shader");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
    DwarfUI::AssetInput<FragmentShaderAsset>(
      mAssetDatabase,
      oglShaderAssets.GetFragmentShaderAsset(),
      "##fragmentShader");
    ImGui::PopItemWidth();

    if (!oglShader.GetShaderLogs().mFragmentShaderLog.empty())
    {
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0F);
      if (ImGui::CollapsingHeader("Fragment Shader Error Log##frag"))
      {
        ImGui::TextWrapped(
          "%s", oglShader.GetShaderLogs().mFragmentShaderLog.c_str());
      }
      ImGui::PopItemWidth();
    }
  }
}