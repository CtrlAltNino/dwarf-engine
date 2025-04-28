#pragma once

#include "Core/UUID.hpp"
#include <filesystem>
#include <utility>

namespace Dwarf
{
  struct PathComponent
  {
  private:
    std::filesystem::path Path;

  public:
    PathComponent(std::filesystem::path path)
      : Path(std::move(path))
    {
    }

    [[nodiscard]] auto
    getPath() const -> const std::filesystem::path&
    {
      return Path;
    }

    void
    setPath(const std::filesystem::path path)
    {
      Path = path;
    }
  };

  /// @brief Entity component holding a unique identifier (UID / GUID).
  struct IDComponent
  {
  private:
    /// @brief The UID of an entity.
    UUID ID;

  public:
    explicit IDComponent(UUID identifier)
      : ID(std::move(identifier))
    {
    }

    /// @brief Retrieves the UID of the entity.
    /// @return The UID.
    [[nodiscard]] auto
    getId() const -> const UUID&
    {
      return ID;
    }
  };

  /// @brief Entity component holding a name (Used for object names).
  struct NameComponent
  {
    /// @brief The entitie's name.
    std::string Name;

    explicit NameComponent(std::string name)
      : Name(std::move(name))
    {
    }
  };
}