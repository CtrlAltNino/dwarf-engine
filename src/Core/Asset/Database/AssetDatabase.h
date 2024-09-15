#pragma once

#include "Core/Asset/AssetReference/IAssetReferenceFactory.h"
#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/GenericComponents.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/UUID.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Model/IModelImporter.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Asset/Shader/IShaderRecompiler.h"
// #include "Core/Asset/Database/AssetReference.h"
#include "Core/Asset/Database/IAssetDirectoryListener.h"

#include "Core/Rendering/Shader/IShader.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler.h"

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
    AssetDirectoryPath m_AssetDirectoryPath;

    /// @brief ECS registry containing entities for every asset in the "/Assets"
    entt::registry m_Registry;

    std::map<std::filesystem::path, std::shared_ptr<IShader>> m_ShaderAssetMap;

    std::shared_ptr<IDwarfLogger>            m_Logger;
    std::shared_ptr<IAssetDirectoryListener> m_AssetDirectoryListener;
    std::shared_ptr<IAssetMetadata>          m_AssetMetadata;
    std::shared_ptr<IModelImporter>          m_ModelImporter;
    std::shared_ptr<IShaderRecompiler>       m_ShaderRecompiler;
    std::shared_ptr<ITextureFactory>         m_TextureFactory;
    std::shared_ptr<IMaterialFactory>        m_MaterialFactory;
    std::shared_ptr<IMaterialIO>             m_MaterialIO;
    std::shared_ptr<IAssetReimporter>        m_AssetReimporter;
    std::shared_ptr<IAssetReferenceFactory>  m_AssetReferenceFactory;

  public:
    /**
     * @brief Construct a new Asset Database object
     */
    AssetDatabase(
      AssetDirectoryPath                       assetDirectoryPath,
      std::shared_ptr<IDwarfLogger>            logger,
      std::shared_ptr<IAssetDirectoryListener> assetDirectoryListener,
      std::shared_ptr<IAssetMetadata>          assetMetadata,
      std::shared_ptr<IModelImporter>          modelImporter,
      std::shared_ptr<IShaderRecompiler>       shaderRecompiler,
      std::shared_ptr<ITextureFactory>         textureFactory,
      std::shared_ptr<IMaterialFactory>        materialFactory,
      std::shared_ptr<IMaterialIO>             materialIO,
      std::shared_ptr<IAssetReimporter>        assetReimporter,
      std::shared_ptr<IAssetReferenceFactory>  assetReferenceFactory);

    /**
     * @brief Destroy the Asset Database object
     */
    ~AssetDatabase() override;

    /**
     * @brief Imports an asset into the asset database.
     * @param assetPath Path to the asset.
     */
    UUID
    Import(const std::filesystem::path& assetPath) override;

    /**
     * @brief Checks if an asset with a given UID exists in the database.
     * @param uid UID of the asset.
     */
    bool
    Exists(const UUID& uid) override;

    /**
     * @brief Checks if an asset with a given path exists in the database.
     * @param path Path to the asset.
     */
    bool
    Exists(const std::filesystem::path& path) override;

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
     * @param path Path to the asset.
     */
    void
    Remove(const std::filesystem::path& path) override;

    /**
     * @brief Reimports all assets in the asset database.
     */
    void
    ReimportAll() override;

    /**
     * @brief Reimports an asset in the asset database.
     * @param assetPath Path to the asset.
     */
    UUID
    Reimport(const std::filesystem::path& assetPath) override;

    /**
     * @brief Renames an asset in the asset database.
     * @param from Old path to the asset.
     * @param to New path to the asset.
     */
    void
    Rename(const std::filesystem::path& from,
           const std::filesystem::path& to) override;

    /**
     * @brief Renames a directory in the asset database.
     * @param from Old path to the directory.
     * @param to New path to the directory.
     */
    void
    RenameDirectory(const std::filesystem::path& from,
                    const std::filesystem::path& to) override;

    entt::registry&
    GetRegistry() override;

  private:
    std::filesystem::path
    GetAssetDirectoryPath() override;

    void
    AddAssetCallback(const std::string& dir, const std::string& filename);

    void
    DeleteAssetCallback(const std::string& dir, const std::string& filename);

    void
    ModifyAssetCallback(const std::string& dir, const std::string& filename);

    void
    MoveAssetCallback(const std::string& dir,
                      const std::string& filename,
                      const std::string& oldFilename);

    /// @brief Recursively imports all found assets in a given directory.
    /// @param directory Absolute path to a directory.
    void
    RecursiveImport(const std::filesystem::path& directory);

    /// @brief Creates an asset reference for an asset at a given path.
    /// @param assetPath Path to the asset.
    /// @return The created asset reference instance.
    template<typename T>
    IAssetReference<T>
    CreateNewAsset(const std::filesystem::path& assetPath)
    {
      std::string           fileName = assetPath.stem().string();
      std::filesystem::path metaDataPath =
        IAssetMetadata::GetMetadataPath(assetPath);

      auto id = UUID();
      if (FileHandler::FileExists(metaDataPath))
      {
        nlohmann::json metaData = m_AssetMetadata->GetMetadata(assetPath);
        std::string    guid = metaData["guid"].get<std::string>();
        id = UUID(guid);
      }
      else
      {
        nlohmann::json metaData;
        metaData["guid"] = id.ToString();
        m_AssetMetadata->SetMetadata(assetPath, metaData);
      }

      // IAssetReference<T> assetReference =
      //   AssetReference<T>(m_Registry.create(), m_Registry);

      IAssetReference<T> assetReference =
        m_AssetReferenceFactory->Create<T>(m_Registry.create(), m_Registry);

      assetReference.template AddAssetComponent<IDComponent>(id);
      assetReference.template AddAssetComponent<NameComponent>(fileName);
      assetReference.template AddAssetComponent<PathComponent>(assetPath);
      return assetReference;
    }

    std::any
    RetrieveImpl(std::type_index type, const UUID& uid) override;

    std::any
    RetrieveImpl(std::type_index              type,
                 const std::filesystem::path& path) override;

    std::any
    CreateAssetReference(std::type_index type, entt::entity entity);
  };
}