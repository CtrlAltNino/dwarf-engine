#pragma once

#include "IProjectSettings.h"

namespace Dwarf
{
  class IProjectSettingsIO
  {
  public:
    virtual ~IProjectSettingsIO() = default;

    virtual std::optional<ProjectSettingsData>
    LoadProjectSettings(std::filesystem::path projectPath) = 0;

    virtual void
    SaveProjectSettings(ProjectSettingsData   projectSettingsData,
                        std::filesystem::path projectPath) = 0;
  };
}