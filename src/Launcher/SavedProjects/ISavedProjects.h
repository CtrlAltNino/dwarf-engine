#pragma once

#include "Core/Base.h"
#include <boost/serialization/strong_typedef.hpp>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(int, ProjectListColumn)

  struct SavedProject
  {
    GraphicsApi           GraphicsApi;
    time_t                LastOpened;
    std::string           ProjectName;
    std::filesystem::path Path;

    bool
    operator==(const SavedProject& other) const
    {
      return ProjectName == other.ProjectName && Path == other.Path &&
             LastOpened == other.LastOpened && GraphicsApi == other.GraphicsApi;
    }
  };

  class ISavedProjects
  {
  public:
    virtual ~ISavedProjects() = default;
    virtual void
    AddProject(const std::filesystem::path& path) = 0;

    virtual void
    OpenAddProjectDialog() = 0;

    virtual void
    RemoveProject(const std::filesystem::path& path) = 0;

    virtual void
    ChangeGraphicsApi(const std::filesystem::path& path, GraphicsApi api) = 0;

    virtual void
    RegisterProjectOpening(const std::filesystem::path& path) = 0;

    virtual std::vector<SavedProject>&
    GetSavedProjects() = 0;
  };
}