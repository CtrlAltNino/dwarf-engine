#pragma once

#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "IShaderCreator.h"
#include "Utilities/FileHandler/IFileHandler.h"

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