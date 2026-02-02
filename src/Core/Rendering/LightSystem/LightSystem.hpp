#pragma once

#include "Core/Scene/Components/LightComponentHandle.hpp"
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
    GLuint                        mDirectionalLightSSBO;
    GLuint                        mPointLightSSBO;

    std::unordered_map<UUID, LightInfo> mLightRegistry;

    LightData mLightData;

    void
    Upload();

  public:
    LightSystem(std::shared_ptr<IDwarfLogger> logger,
                std::shared_ptr<ILoadedScene> loadedScene);
    ~LightSystem() override;

    void
    UpdateRegistry() override;

    void
    UpdateLightData() override;

    [[nodiscard]] auto
    GetLightData() -> LightData& override;

    [[nodiscard]] auto
    GetLightRegistry() -> std::unordered_map<UUID, LightInfo>& override;

    void
    Bind() const override;

    void
    OnSceneLoad() override;

    void
    OnSceneUnload() override;
  };
}