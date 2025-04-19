#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "IShaderCreator.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"

namespace Dwarf
{
  class ShaderCreator : public IShaderCreator
  {
  private:
    AssetDirectoryPath            mAssetDirectoryPath;
    std::shared_ptr<IFileHandler> mFileHandler;

  public:
    ShaderCreator(const AssetDirectoryPath&     assetDirectoryPath,
                  std::shared_ptr<IFileHandler> fileHandler);
    ~ShaderCreator() override = default;

    /**
     * @brief Create a new shader asset
     *
     * @param type Type of the shader
     * @param source Source of the shader
     * @param directoryPath Optional directory path
     */
    void
    CreateShaderAsset(
      ShaderType                           type,
      ShaderSource                         source,
      std::optional<std::filesystem::path> directoryPath) override;
  };
} // namespace Dwarf