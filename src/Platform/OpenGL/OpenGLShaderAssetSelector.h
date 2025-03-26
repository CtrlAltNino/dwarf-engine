#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/ShaderAssetSelector/IShaderAssetSelector.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class OpenGLShaderAssetSelector : public IShaderAssetSelector
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                    mShaderSourceCollectionFactory;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;

    std::shared_ptr<IShader> mSelectedShader;

    std::unique_ptr<IAssetReference> mVertexShaderAsset;
    std::unique_ptr<IAssetReference> mTessellationControlShaderAsset;
    std::unique_ptr<IAssetReference> mTessellationEvaluationShaderAsset;
    std::unique_ptr<IAssetReference> mGeometryShaderAsset;
    std::unique_ptr<IAssetReference> mFragmentShaderAsset;

  public:
    OpenGLShaderAssetSelector(std::shared_ptr<IDwarfLogger> logger,
                              std::shared_ptr<IShaderSourceCollectionFactory>
                                shaderSourceCollectionFactory,
                              std::shared_ptr<IAssetDatabase> assetDatabase);
    ~OpenGLShaderAssetSelector() override;

    /**
     * @brief Sets the current shader to select sources for
     *
     * @param shader Selected shader
     */
    void
    SetCurrentShader(std::shared_ptr<IShader> shader) override;

    /**
     * @brief Creates a shader source collection from the current selection
     *
     * @return Unique pointer to the created shader source colleciton
     */
    auto
    GetCurrentSelection() -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Renders the selection UI
     *
     */
    void
    Render() override;
  };
}