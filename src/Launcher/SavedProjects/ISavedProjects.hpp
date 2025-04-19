#pragma once

#include "Core/Base.hpp"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(int, ProjectListColumn)

  struct SavedProject
  {
    GraphicsApi           GraphicsApi;
    time_t                LastOpened;
    std::string           ProjectName;
    std::filesystem::path Path;

    auto
    operator==(const SavedProject& other) const -> bool
    {
      return ProjectName == other.ProjectName && Path == other.Path &&
             LastOpened == other.LastOpened && GraphicsApi == other.GraphicsApi;
    }
  };

  /**
   * @brief Class that manages the list of saved projects of the user
   *
   */
  class ISavedProjects
  {
  public:
    virtual ~ISavedProjects() = default;

    /**
     * @brief Adds a project to the list by path
     *
     * @param path Path to a project directory
     */
    virtual void
    AddProject(const std::filesystem::path& path) = 0;

    /**
     * @brief Opens a dialog to find and add a project
     *
     */
    virtual void
    OpenAddProjectDialog() = 0;

    /**
     * @brief Removes a project from the list
     *
     * @param path Path to a project directory
     */
    virtual void
    RemoveProject(const std::filesystem::path& path) = 0;

    /**
     * @brief Changes the graphics api of a project
     *
     * @param path Path to the project directory
     * @param api Api to change the project to
     */
    virtual void
    ChangeGraphicsApi(const std::filesystem::path& path, GraphicsApi api) = 0;

    /**
     * @brief Updates the time stamp for the last time a project was opened
     *
     * @param path Path to a project directory
     */
    virtual void
    RegisterProjectOpening(const std::filesystem::path& path) = 0;

    /**
     * @brief Returns the list of saved projects
     *
     * @return List of saved projects
     */
    [[nodiscard]] virtual auto
    GetSavedProjects() -> std::vector<SavedProject>& = 0;
  };
}