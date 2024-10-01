#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "pch.h"
#include "Core/Base.h"
#include "Core/UUID.h"
#include <nlohmann/json_fwd.hpp>

namespace Dwarf
{
  class IProjectSettings
  {
  public:
    virtual ~IProjectSettings() = default;

    virtual void
    Save() = 0;

    virtual void
    UpdateProjectName(const std::string& projectName) = 0;

    virtual std::string
    GetProjectName() const = 0;

    virtual void
    UpdateLastOpenedTimeStamp(const time_t& projectLastOpenedDate) = 0;

    virtual const time_t&
    GetLastOpenedTimeStamp() const = 0;

    virtual void
    UpdateGraphicsApi(const GraphicsApi& graphicsApi) = 0;

    virtual const GraphicsApi&
    GetGraphicsApi() const = 0;

    virtual void
    UpdateLastOpenedScene(const UUID& lastOpenedScene) = 0;

    virtual const std::optional<UUID>&
    GetLastOpenedScene() const = 0;

    virtual void
    UpdateSerializedView(const nlohmann::json& serializedView) = 0;

    virtual nlohmann::json
    GetSerializedView() const = 0;
  };
}