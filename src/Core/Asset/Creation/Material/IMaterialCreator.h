#pragma once

namespace Dwarf
{
  class IMaterialCreator
  {
  public:
    virtual ~IMaterialCreator() = default;

    /**
     * @brief Create a new Material asset on disk
     *
     * @param assetPath If a path is supplied, the new asset will be written
     * there
     */
    virtual void
    CreateMaterialAsset(
      std::optional<std::filesystem::path> assetPath = std::nullopt) = 0;
  };
} // namespace Dwarf