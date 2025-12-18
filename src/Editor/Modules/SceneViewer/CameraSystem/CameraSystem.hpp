#pragma once

#include "Core/Scene/Components/CameraComponentHandle.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "ICameraSystem.hpp"
#include "Input/IInputManager.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <glm/ext/matrix_float4x4.hpp>

namespace Dwarf
{
  class CameraSystem : public ICameraSystem
  {
  private:
    std::shared_ptr<IDwarfLogger>        mLogger;
    std::shared_ptr<IInputManager>       mInputManager;
    std::shared_ptr<ILoadedScene>        mLoadedScene;
    CameraSystemProperties               mProperties;
    std::optional<CameraComponentHandle> mCamera;
    glm::mat4                            mCachedViewMat = glm::mat4(0);
    glm::mat4                            mCachedProjectionMat = glm::mat4(0);
    glm::vec3                            mCachedPos = glm::vec3(0);

  public:
    CameraSystem(std::shared_ptr<IDwarfLogger>  logger,
                 std::shared_ptr<IInputManager> inputManager,
                 std::shared_ptr<ILoadedScene>  loadedScene);

    CameraSystem(nlohmann::json                 serializedData,
                 std::shared_ptr<IDwarfLogger>  logger,
                 std::shared_ptr<IInputManager> inputManager,
                 std::shared_ptr<ILoadedScene>  loadedScene);
    ~CameraSystem() override;

    void
    SetCamera(CameraComponentHandle handle) override;

    auto
    GetCamera() -> CameraComponentHandle override;

    [[nodiscard]] auto
    HasCamera() const -> bool override;

    [[nodiscard]] auto
    GetViewMatrix() const -> glm::mat4x4 override;

    [[nodiscard]] auto
    GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4 override;

    [[nodiscard]] auto
    GetPosition() const -> glm::vec3 override;

    [[nodiscard]] auto
    GetNearplane() const -> float override;

    [[nodiscard]] auto
    GetFarplane() const -> float override;

    [[nodiscard]] auto
    GetFov() const -> float override;

    void
    SetNearplane(float nearplane) override;

    void
    SetFarplane(float farplane) override;

    void
    SetFov(float fov) override;

    void
    OnUpdate(double deltaTime) override;

    [[nodiscard]] auto
    ScreenToWorld(glm::vec2 const& screenPos, glm::vec2 const& viewport) const
      -> glm::vec3 override;

    auto
    Serialize() -> nlohmann::json override;
  };
}