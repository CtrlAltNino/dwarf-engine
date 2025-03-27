#include "pch.h"

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "OpenGLShaderAssetSourceContainer.h"

namespace Dwarf
{
  OpenGLShaderAssetSourceContainer::OpenGLShaderAssetSourceContainer(
    std::shared_ptr<IAssetDatabase> assetDatabase,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                             shaderSourceCollectionFactory,
    std::unique_ptr<IShaderSourceCollection> shaderSources)
    : mAssetDatabase(assetDatabase)
    , mShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
    for (std::unique_ptr<IAssetReference>& shaderSource :
         shaderSources->GetShaderSources())
    {
      // std::visit(HandleShaderSourceVisitor(*this), shaderSource);
      switch (shaderSource->GetType())
      {
        case Dwarf::ASSET_TYPE::VERTEX_SHADER:
          mVertexShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::FRAGMENT_SHADER:
          mFragmentShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::GEOMETRY_SHADER:
          mGeometryShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::TESC_SHADER:
          mTessellationControlShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::TESE_SHADER:
          mTessellationEvaluationShaderAsset = std::move(shaderSource);
          break;
        default: break;
      }
    }
  }

  auto
  OpenGLShaderAssetSourceContainer::GetShaderSources()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources;

    if (mVertexShaderAsset.has_value())
    {
      shaderSources.emplace_back(
        mAssetDatabase.lock()->Retrieve(mVertexShaderAsset->get()->GetUID()));
    }

    if (mTessellationControlShaderAsset.has_value())
    {
      shaderSources.emplace_back(mAssetDatabase.lock()->Retrieve(
        mTessellationControlShaderAsset->get()->GetUID()));
    }

    if (mTessellationEvaluationShaderAsset.has_value())
    {
      shaderSources.emplace_back(mAssetDatabase.lock()->Retrieve(
        mTessellationEvaluationShaderAsset->get()->GetUID()));
    }

    if (mGeometryShaderAsset.has_value())
    {
      shaderSources.emplace_back(
        mAssetDatabase.lock()->Retrieve(mGeometryShaderAsset->get()->GetUID()));
    }

    if (mFragmentShaderAsset.has_value())
    {
      shaderSources.emplace_back(
        mAssetDatabase.lock()->Retrieve(mFragmentShaderAsset->get()->GetUID()));
    }

    return mShaderSourceCollectionFactory.lock()->CreateShaderSourceCollection(
      shaderSources);
  }

  auto
  OpenGLShaderAssetSourceContainer::GetVertexShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mVertexShaderAsset;
  }

  auto
  OpenGLShaderAssetSourceContainer::GetFragmentShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mFragmentShaderAsset;
  }

  auto
  OpenGLShaderAssetSourceContainer::GetGeometryShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mGeometryShaderAsset;
  }

  auto
  OpenGLShaderAssetSourceContainer::GetTessellationControlShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mTessellationControlShaderAsset;
  }

  auto
  OpenGLShaderAssetSourceContainer::GetTessellationEvaluationShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mTessellationEvaluationShaderAsset;
  }
}