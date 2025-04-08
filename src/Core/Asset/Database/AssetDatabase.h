#pragma once

#include "Core/Asset/AssetReference/IAssetReferenceFactory.h"
#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Database/IAssetDirectoryListener.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Asset/Model/IModelImporter.h"
#include "Core/Asset/Shader/IShaderRecompiler.h"
#include "Core/Base.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/UUID.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "Window/IWindow.h"
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <string>

namespace Dwarf
{
  /**
   * @class AssetDatabase
   * @brief Dwarf Engines asset database system. It handles an ECS registry
   * containing entities for every usable Asset inside the "/Assets" directory,
   * and reimports them when changes on the disk happen.
   *
   * It also provides methods to import, reimport, create remove and rename
   * assets.
   *
   */
  class AssetDatabase : public IAssetDatabase
  {
  private:
    /// @brief Absolute path to the "/Asset" directory.
    AssetDirectoryPath mAssetDirectoryPath;

    /// @brief ECS registry containing entities for every asset in the "/Assets"
    entt::registry mRegistry;

    std::map<std::filesystem::path, std::shared_ptr<IShader>> mShaderAssetMap;

    GraphicsApi                              mGraphicsApi;
    std::shared_ptr<IDwarfLogger>            mLogger;
    std::shared_ptr<IAssetDirectoryListener> mAssetDirectoryListener;
    std::shared_ptr<IAssetMetadata>          mAssetMetadata;
    std::shared_ptr<IModelImporter>          mModelImporter;
    std::shared_ptr<IShaderRecompiler>       mShaderRecompiler;
    std::shared_ptr<ITextureFactory>         mTextureFactory;
    std::shared_ptr<IMaterialFactory>        mMaterialFactory;
    std::shared_ptr<IMaterialIO>             mMaterialIO;
    std::shared_ptr<IAssetReimporter>        mAssetReimporter;
    std::shared_ptr<IAssetReferenceFactory>  mAssetReferenceFactory;
    std::shared_ptr<IFileHandler>            mFileHandler;
    std::shared_ptr<IShaderRegistry>         mShaderRegistry;

  public:
    /**
     * @brief Construct a new Asset Database object
     */
    AssetDatabase(
      const AssetDirectoryPath&                assetDirectoryPath,
      GraphicsApi                              graphicsApi,
      std::shared_ptr<IDwarfLogger>            logger,
      std::shared_ptr<IAssetDirectoryListener> assetDirectoryListener,
      std::shared_ptr<IAssetMetadata>          assetMetadata,
      std::shared_ptr<IModelImporter>          modelImporter,
      std::shared_ptr<IShaderRecompiler>       shaderRecompiler,
      std::shared_ptr<ITextureFactory>         textureFactory,
      std::shared_ptr<IMaterialFactory>        materialFactory,
      std::shared_ptr<IMaterialIO>             materialIO,
      std::shared_ptr<IAssetReimporter>        assetReimporter,
      std::shared_ptr<IAssetReferenceFactory>  assetReferenceFactory,
      std::shared_ptr<IFileHandler>            fileHandler,
      std::shared_ptr<IShaderRegistry>         shaderRegistry,
      std::shared_ptr<IWindow>                 window);

    /**
     * @brief Destroy the Asset Database object
     */
    ~AssetDatabase() override;

    /**
     * @brief Imports an asset into the asset database.
     * @param assetPath Path to the asset.
     */
    auto
    Import(const std::filesystem::path& assetPath) -> UUID override;

    /**
     * @brief Opens a Dialog to import assets
     */
    void
    ImportDialog() override;

    /**
     * @brief Checks if an asset with a given UID exists in the database.
     * @param uid UID of the asset.
     */
    auto
    Exists(const UUID& uid) -> bool override;

    /**
     * @brief Checks if an asset with a given path exists in the database.
     * @param path Path to the asset.
     */
    auto
    Exists(const std::filesystem::path& assetPath) -> bool override;

    /// @brief Clears the asset database.
    /**
     * @brief Clears the asset database.
     */
    void
    Clear() override;

    /**
     * @brief Removes an asset from the asset database.
     * @param uid UID of the asset.
     */
    void
    Remove(const UUID& uid) override;

    /**
     * @brief Removes an asset from the asset database.
     * @param assetPath Path to the asset.
     */
    void
    Remove(const std::filesystem::path& assetPath) override;

    /**
     * @brief Reimports all assets in the asset database.
     */
    void
    ReimportAll() override;

    /**
     * @brief Reimports an asset in the asset database.
     * @param assetPath Path to the asset.
     */
    void
    Reimport(const std::filesystem::path& assetPath) override;

    /**
     * @brief Renames an asset in the asset database.
     * @param from Old path to the asset.
     * @param to New path to the asset.
     */
    void
    Rename(const std::filesystem::path& fromPath,
           const std::filesystem::path& toPath) override;

    /**
     * @brief Renames a directory in the asset database.
     * @param from Old path to the directory.
     * @param to New path to the directory.
     */
    void
    RenameDirectory(const std::filesystem::path& fromPath,
                    const std::filesystem::path& toPath) override;

    /**
     * @brief Returns a reference to the underlying entt registry
     *
     * @return entt::registry reference
     */
    auto
    GetRegistry() -> entt::registry& override;

    /**
     * @brief Retrieves an asset from the asset database.
     * @param uid UID of the asset.
     */
    auto
    Retrieve(const UUID& uid) -> std::unique_ptr<IAssetReference> override;

    /**
     * @brief Retrieves an asset from the asset database.
     * @param path Path to the asset.
     */
    auto
    Retrieve(const std::filesystem::path& assetPath)
      -> std::unique_ptr<IAssetReference> override;

    /**
     * @brief Returns the path to the projects asset directory
     *
     * @return The path of the asset directory
     */
    auto
    GetAssetDirectoryPath() -> std::filesystem::path override;

  private:
    /**
     * @brief EFSW Callback for when a file is added to the project
     *
     * @param dir Directory in which an asset is added to
     * @param filename File name of the added asset
     */
    void
    AddAssetCallback(const std::string& dir, const std::string& filename);

    /**
     * @brief EFSW Callback for when a file in the project is deleted
     *
     * @param dir Directory in which the file was removed
     * @param filename File name of the deleted file
     */
    void
    DeleteAssetCallback(const std::string& dir, const std::string& filename);

    /**
     * @brief EFSW Callback for when an asset is modified on disk
     *
     * @param dir Directory of the modified file
     * @param filename File name of the modified file
     */
    void
    ModifyAssetCallback(const std::string& dir, const std::string& filename);

    /**
     * @brief EFSW Callback for when an asset is moved
     *
     * @param dir New directory of the asset file
     * @param filename File name of the file
     * @param oldFilename Old path of the file
     */
    void
    MoveAssetCallback(const std::string& dir,
                      const std::string& filename,
                      const std::string& oldFilename);

    /// @brief Recursively imports all found assets in a given directory.
    /// @param directory Absolute path to a directory.
    void
    GatherAssetPaths(const std::filesystem::path&        directoryPath,
                     std::vector<std::filesystem::path>& materialPaths,
                     std::vector<std::filesystem::path>& otherPaths);

    /**
     * @brief Imports all default assets
     *
     */
    void
    ImportDefaultAssets();

    /**
     * @brief Imports the default shaders
     *
     */
    void
    ImportDefaultShaders();

    /**
     * @brief Imports all default textures
     *
     */
    void
    ImportDefaultTextures();

    /**
     * @brief Imports all default materials
     *
     */
    void
    ImportDefaultMaterials();

    /**
     * @brief Imports all default models
     *
     */
    void
    ImportDefaultModels();

    void
    HotReloadShaders(const std::filesystem::path& shaderAssetPath);
  };
}