#pragma once

namespace Dwarf
{
  class IAssetMetaData
  {
  private:
    static constexpr const char* META_DATA_EXTENSION = ".dmeta";

  public:
    virtual ~IAssetMetaData() = default;

    static std::filesystem::path
    GetMetaDataPath(std::filesystem::path assetPath)
    {
      return assetPath.concat(META_DATA_EXTENSION);
    }

    static bool
    IsMetaDataPath(std::filesystem::path path)
    {
      return path.extension() == META_DATA_EXTENSION;
    }

    virtual nlohmann::json
    GetMetaData(const std::filesystem::path& assetPath) const = 0;
    virtual void
    SetMetaData(const std::filesystem::path& assetPath,
                const nlohmann::json&        value) = 0;
    virtual void
    RemoveMetaData(const std::filesystem::path& key) = 0;
    virtual void
    Rename(const std::filesystem::path& from,
           const std::filesystem::path& to) = 0;
  };
}