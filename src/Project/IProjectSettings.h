#pragma once

#include "dpch.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/Base.h"

namespace Dwarf
{
  class IProjectSettings
  {
  public:
    virtual ~IProjectSettings() = default;

    virtual void
    Save() = 0;

    virtual void
    SetProjectName(const std::string& projectName) = 0;

    virtual std::string
    GetProjectName() const = 0;

    virtual void
    SetLastOpenedTimeStamp(const time_t& projectLastOpenedDate) = 0;

    virtual const time_t&
    GetLastOpenedTimeStamp() const = 0;

    virtual void
    SetGraphicsApi(const GraphicsApi& graphicsApi) = 0;

    virtual const GraphicsApi&
    GetGraphicsApi() const = 0;

    virtual void
    SetLastOpenedScene(const AssetReference<SceneAsset>& lastOpenedScene) = 0;

    virtual const AssetReference<SceneAsset>&
    GetLastOpenedScene() const = 0;
  };
}