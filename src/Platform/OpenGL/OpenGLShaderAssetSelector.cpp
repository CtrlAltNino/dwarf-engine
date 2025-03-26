#include "pch.h"

#include "OpenGLShaderAssetSelector.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "UI/DwarfUI.h"


namespace Dwarf
{
  OpenGLShaderAssetSelector::OpenGLShaderAssetSelector(
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                    shaderSourceCollectionFactory,
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : mLogger(std::move(logger))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
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
  OpenGLShaderAssetSelector::SetCurrentShader(std::shared_ptr<IShader> shader)
  {
    mSelectedShader = shader;

    auto& oglShader = dynamic_cast<OpenGLShader&>(*shader);

    mVertexShaderAsset =
      oglShader.GetVertexShaderAsset().has_value()
        ? mAssetDatabase->Retrieve(
            oglShader.GetVertexShaderAsset().value()->GetUID())
        : nullptr;

    mTessellationControlShaderAsset =
      oglShader.GetTessellationControlShaderAsset().has_value()
        ? mAssetDatabase->Retrieve(
            oglShader.GetTessellationControlShaderAsset().value()->GetUID())
        : nullptr;

    mTessellationEvaluationShaderAsset =
      oglShader.GetTessellationEvaluationShaderAsset().has_value()
        ? mAssetDatabase->Retrieve(
            oglShader.GetTessellationEvaluationShaderAsset().value()->GetUID())
        : nullptr;

    mGeometryShaderAsset =
      oglShader.GetGeometryShaderAsset().has_value()
        ? mAssetDatabase->Retrieve(
            oglShader.GetGeometryShaderAsset().value()->GetUID())
        : nullptr;

    mFragmentShaderAsset =
      oglShader.GetFragmentShaderAsset().has_value()
        ? mAssetDatabase->Retrieve(
            oglShader.GetFragmentShaderAsset().value()->GetUID())
        : nullptr;
  }

  auto
  OpenGLShaderAssetSelector::GetCurrentSelection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources;

    if (mVertexShaderAsset != nullptr)
    {
      shaderSources.emplace_back(
        mAssetDatabase->Retrieve(mVertexShaderAsset->GetUID()));
    }

    if (mTessellationControlShaderAsset != nullptr)
    {
      shaderSources.emplace_back(
        mAssetDatabase->Retrieve(mTessellationControlShaderAsset->GetUID()));
    }

    if (mTessellationEvaluationShaderAsset)
    {
      shaderSources.emplace_back(
        mAssetDatabase->Retrieve(mTessellationEvaluationShaderAsset->GetUID()));
    }

    if (mGeometryShaderAsset)
    {
      shaderSources.emplace_back(
        mAssetDatabase->Retrieve(mGeometryShaderAsset->GetUID()));
    }

    if (mFragmentShaderAsset)
    {
      shaderSources.emplace_back(
        mAssetDatabase->Retrieve(mFragmentShaderAsset->GetUID()));
    }

    return mShaderSourceCollectionFactory->CreateShaderSourceCollection(
      shaderSources);
  }

  void
  OpenGLShaderAssetSelector::Render()
  {
    auto& oglShader = dynamic_cast<OpenGLShader&>(*mSelectedShader);

    ImGui::TextWrapped("Vertex Shader");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
    DwarfUI::AssetInput<VertexShaderAsset>(
      mAssetDatabase, mVertexShaderAsset, "##vertexShader");
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
      mTessellationControlShaderAsset,
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
      mTessellationEvaluationShaderAsset,
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
      mAssetDatabase, mGeometryShaderAsset, "##geometryShader");
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
      mAssetDatabase, mFragmentShaderAsset, "##fragmentShader");
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