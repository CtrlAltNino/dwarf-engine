#pragma once

#include "Core/Base.h"
namespace Dwarf
{
  /// @brief Enum defining the available project templates.
  enum class ProjectTemplate
  {
    Blank,
    Olaf,
    Sponza,
    DamagedHelmet
  };

  /// @brief Interface for the project creator.
  class IProjectCreator
  {
  public:
    virtual ~IProjectCreator() = default;

    virtual std::filesystem::path
    GetDefaultProjectPath() const = 0;

    virtual void
    CreateProject(std::string           projectName,
                  std::filesystem::path projectPath,
                  GraphicsApi           graphicsApi,
                  ProjectTemplate       projectTemplate) = 0;
  };
} // namespace Dwarf