#pragma once

#include "Core/Base.h"
#include "Core/UUID.h"
#include <filesystem>

namespace Dwarf
{
  struct PathComponent
  {
  private:
    /// @brief Path to the asset.
    std::filesystem::path Path;

  public:
    PathComponent() = default;
    PathComponent(const PathComponent&) = default;
    explicit PathComponent(const std::filesystem::path& path)
      : Path(path)
    {
    }

    /// @brief Retrieves the path to the asset.
    /// @return The path.
    const std::filesystem::path&
    GetPath() const
    {
      return Path;
    }
  };

  /// @brief Entity component holding a unique identifier (UID / GUID).
  struct IDComponent
  {
  private:
    /// @brief The UID of an entity.
    UUID ID;

  public:
    IDComponent() = default;
    IDComponent(const IDComponent&) = default;
    explicit IDComponent(const UUID& other)
      : ID(other)
    {
    }

    /// @brief Retrieves the UID of the entity.
    /// @return The UID.
    const UUID&
    GetID() const
    {
      return ID;
    }
  };

  /// @brief Entity component holding a name (Used for object names).
  struct NameComponent
  {
    /// @brief The entitie's name.
    std::string Name;

    NameComponent() = default;
    NameComponent(const NameComponent&) = default;
    explicit NameComponent(const std::string& name)
      : Name(name)
    {
    }
  };
}