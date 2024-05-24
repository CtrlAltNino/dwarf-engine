#pragma once
#include "Core/Asset/IAssetDatabase.h"
#include "IAssetMetaData.h"
#include "IMaterialSerializer.h"
#include "Project/ProjectTypes.h"
#include <entt/entt.hpp>

#include "Core/UID.h"
#include "Core/Asset/AssetReference.h"
#include "Utilities/FileHandler.h"
#include "Core/Rendering/Shader.h"
#include "Core/Asset/IAssetDirectoryListener.h"
#include "Core/Asset/AssetTypes.h"

namespace Dwarf
{
  /// @brief Dwarf Engines asset database system. It handles an ECS registry
  /// containing entities for every usable Asset inside the "/Assets" directory.
  class AssetDatabase : public IAssetDatabase
  {
  private:
    /// @brief Absolute path to the "/Asset" directory.
    AssetDirectoryPath m_AssetDirectoryPath;

    std::shared_ptr<entt::registry> m_Registry;

    std::map<std::filesystem::path, std::shared_ptr<Shader>> m_ShaderAssetMap;

    std::vector<std::shared_ptr<Shader>> m_ShaderRecompilationStack;

    std::shared_ptr<IAssetDirectoryListener> m_AssetDirectoryListener;
    std::shared_ptr<IAssetMetaData>          m_AssetMetaData;
    std::shared_ptr<IMaterialSerializer>     m_MaterialSerializer;

    /// @brief Recursively imports all found assets in a given directory.
    /// @param directory Absolute path to a directory.
    void
    RecursiveImport(std::filesystem::path const& directory);

    void
    CompileShaders();

    /// @brief Creates an asset reference for an asset at a given path.
    /// @param assetPath Path to the asset.
    /// @return The created asset reference instance.
    template<typename T>
    AssetReference<T>
    CreateAssetReference(std::filesystem::path const& assetPath)
    {
      std::string           fileName = assetPath.stem().string();
      std::filesystem::path metaDataPath =
        IAssetMetaData::GetMetaDataPath(assetPath);

      auto id = UID();
      if (FileHandler::CheckIfFileExists(metaDataPath.string().c_str()))
      {
        nlohmann::json metaData = m_AssetMetaData->GetMetaData(assetPath);
        id = UID(metaData["guid"]);
      }
      else
      {
        nlohmann::json metaData;
        metaData["guid"] = (uint64_t)id;
        m_AssetMetaData->SetMetaData(assetPath, metaData);
      }

      return AssetReference<T>(
        m_Registry->create(), fileName, m_Registry, id, assetPath);
    }

  public:
    AssetDatabase(
      ProjectPath const&                       projectPath,
      std::shared_ptr<IAssetDirectoryListener> assetDirectoryListener,
      std::shared_ptr<IAssetMetaData>          assetMetaData,
      std::shared_ptr<IMaterialSerializer>     materialSerializer);

    ~AssetDatabase() override;

    /// @brief Imports an asset into the asset database.
    /// @param assetPath Path to the asset.
    std::shared_ptr<UID>
    Import(std::filesystem::path const& assetPath) override;

    bool
    Exists(std::shared_ptr<UID> uid) override;
    bool
    Exists(std::filesystem::path const& path) override;

    /// @brief Clears the asset database.
    void
    Clear() override;
    void
    Remove(std::shared_ptr<UID> uid) override;
    void
    Remove(std::filesystem::path const& path) override;

    void
    ReimportAll() override;
    std::shared_ptr<UID>
    Reimport(std::filesystem::path const& assetPath) override;

    void
    Rename(std::filesystem::path const& from,
           std::filesystem::path const& to) override;

    void
    RenameDirectory(std::filesystem::path const& from,
                    std::filesystem::path const& to) override;

    void
    CreateNewMaterialAsset() override;
    void
    CreateNewMaterialAsset(std::filesystem::path const& path) override;

    std::filesystem::path
    GetAssetDirectoryPath() override;

    void
    RecompileShaders() override;

    void
    AddShaderWatch(std::filesystem::path const& shaderAssetPath,
                   std::shared_ptr<Shader>      shader) override;
    void
    RemoveShaderWatch(std::filesystem::path const& shaderAssetPath) override;

    void
    AddShaderToRecompilationQueue(std::filesystem::path const& path) override;
    void
    AddShaderToRecompilationQueue(std::shared_ptr<Shader> shader) override;

    void
    AddAssetCallback(const std::string& dir, const std::string& filename);

    void
    DeleteAssetCallback(const std::string& dir, const std::string& filename);

    void
    ModifyAssetCallback(const std::string& dir, const std::string& filename);

    void
    MoveAssetCallback(const std::string& dir,
                      const std::string& filename,
                      std::string        oldFilename);

  private:
    std::shared_ptr<void>
    RetrieveImpl(std::type_index type, std::shared_ptr<UID> uid) override;

    std::shared_ptr<void>
    RetrieveImpl(std::type_index              type,
                 std::filesystem::path const& path) override;

    std::shared_ptr<void>
    CreateAssetReference(std::type_index type, entt::entity entity);
  };
}