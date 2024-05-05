#pragma once

#include "Core/Base.h"
#include "Core/UID.h"

namespace Dwarf
{
  struct PathComponent
  {
    /// @brief Path to the asset.
    std::filesystem::path Path;

    PathComponent() = default;
    PathComponent(const PathComponent&) = default;
    explicit PathComponent(const std::filesystem::path& path)
      : Path(path)
    {
    }
  };

  /// @brief Entity component holding a unique identifier (UID / GUID).
  struct IDComponent
  {
    /// @brief The UID of an entity.
    std::shared_ptr<UID> ID;
    IDComponent() = default;
    IDComponent(const IDComponent&) = default;
    explicit IDComponent(const UID& other)
      : ID(std::make_shared<UID>(other))
    {
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