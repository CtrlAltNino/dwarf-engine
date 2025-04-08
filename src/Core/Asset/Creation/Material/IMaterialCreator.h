#pragma once

#include <filesystem>
#include <optional>

namespace Dwarf
{
  enum MaterialType
  {
    PbrMaterial,
    UnlitMaterial
  };

  class IMaterialCreator
  {
  public:
    virtual ~IMaterialCreator() = default;

    /**
     * @brief Create a new Material asset on disk
     *
     * @param directory If a path is supplied, the new asset will be written
     * there
     */
    virtual void
    CreateMaterialAsset(
      MaterialType                         materialType,
      std::optional<std::filesystem::path> directoryPath = std::nullopt) = 0;
  };
} // namespace Dwarf