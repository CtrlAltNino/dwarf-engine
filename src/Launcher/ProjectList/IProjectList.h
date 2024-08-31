#pragma once

#include "Core/Base.h"
#include "Launcher/IProjectLauncher.h"
#include <boost/serialization/strong_typedef.hpp>
namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(int, ProjectListColumn)
  class IProjectList
  {
  public:
    virtual void
    AddProject(const std::filesystem::path& path) = 0;

    virtual void
    OpenAddProjectDialog() = 0;

    // virtual void
    // RemoveProject(const std::string& name) = 0;

    virtual void
    RemoveProject(const std::filesystem::path& path) = 0;

    virtual void
    ChangeGraphicsApi(const std::filesystem::path& path, GraphicsApi api) = 0;

    virtual void
    RegisterProjectOpening(const std::filesystem::path& path) = 0;

    // virtual void
    // ClearProjectList() = 0;

    virtual std::vector<ProjectInformation>&
    GetProjectList() = 0;
  };
}