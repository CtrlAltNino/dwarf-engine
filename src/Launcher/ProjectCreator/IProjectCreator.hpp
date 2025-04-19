#pragma once

#include "Core/Base.hpp"

namespace Dwarf
{
  /// @brief Enum defining the available project templates.
  enum class ProjectTemplate : uint8_t
  {
    Blank,
    Olaf,
    CrytekSponza,
    ScifiHelmet,
    CornellBox
  };

  /// @brief Interface for the project creator.
  class IProjectCreator
  {
  public:
    virtual ~IProjectCreator() = default;

    /**
     * @brief Gets the default project path
     *
     * @return Path to the default location to create projects at
     */
    [[nodiscard]] virtual auto
    GetDefaultProjectPath() const -> std::filesystem::path = 0;

    /**
     * @brief Creates a new project with the provided properties
     *
     * @param projectName Name of the project
     * @param projectPath Directory path
     * @param graphicsApi Graphics api to use
     * @param projectTemplate Project template to base the project off of
     */
    virtual void
    CreateProject(std::string           projectName,
                  std::filesystem::path projectPath,
                  GraphicsApi           graphicsApi,
                  ProjectTemplate       projectTemplate) = 0;
  };
} // namespace Dwarf