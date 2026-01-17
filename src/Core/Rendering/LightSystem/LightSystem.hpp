#pragma once

#include "Core/Scene/ISceneObserver.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "ILightSystem.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "glad/glad.h"

namespace Dwarf
{
  class LightSystem
    : public ILightSystem
    , public ILoadedSceneObserver
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<ILoadedScene> mLoadedScene;
    GLuint                        directionalLightSSBO;
    GLuint                        pointLightSSBO;

    LightData mLightData;
    LightData mTempLightData;

    void
    Upload();

  public:
    LightSystem(std::shared_ptr<IDwarfLogger> logger,
                std::shared_ptr<ILoadedScene> loadedScene);
    ~LightSystem() override;

    void
    Update() override;

    [[nodiscard]] auto
    GetLightData() -> LightData& override;

    void
    Bind() const override;

    void
    OnSceneLoad() override;

    void
    OnSceneUnload() override;
  };
}