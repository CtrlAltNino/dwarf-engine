#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Rendering/Material/ShaderAssetSourceContainer/IShaderAssetSourceContainer.hpp"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/ShaderAssetSelector/IShaderAssetSelector.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class OpenGLShaderAssetSelector : public IShaderAssetSelector
  {
  private:
    std::shared_ptr<IDwarfLogger>   mLogger;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;

    std::shared_ptr<IShader> mSelectedShader;
    std::optional<
      std::reference_wrapper<std::unique_ptr<IShaderAssetSourceContainer>>>
      mSelectedShaderAssetSourceContainer;

  public:
    OpenGLShaderAssetSelector(std::shared_ptr<IDwarfLogger>   logger,
                              std::shared_ptr<IAssetDatabase> assetDatabase);
    ~OpenGLShaderAssetSelector() override;

    /**
     * @brief Sets the current shader to select sources for
     *
     * @param shader Selected shader
     */
    void
    SetCurrentShader(std::shared_ptr<IShader> shader,
                     std::unique_ptr<IShaderAssetSourceContainer>&
                       selectedShaderAssetSourceContainer) override;

    /**
     * @brief Renders the selection UI
     *
     */
    void
    Render() override;
  };
}