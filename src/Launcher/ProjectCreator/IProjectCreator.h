#pragma once

#include "Core/Base.h"
namespace Dwarf
{
  /// @brief Enum defining the available project templates.
  enum class ProjectTemplate
  {
    Blank,
    Sponza
  };

  /// @brief Interface for the project creator.
  class IProjectCreator
  {
  public:
    virtual std::filesystem::path
    GetDefaultProjectPath() = 0;

    virtual void
    CreateProject(std::string           projectName,
                  std::filesystem::path projectPath,
                  GraphicsApi           graphicsApi,
                  ProjectTemplate       projectTemplate) = 0;
  };
} // namespace Dwarf