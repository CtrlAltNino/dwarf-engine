#pragma once

#include "Core/Scene/Camera/ICamera.hpp"
#include "Utilities/ISerializable.hpp"
#include <entt/entt.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <memory>
#include <nlohmann/json_fwd.hpp>

namespace Dwarf
{
  /**
   * @brief ECS component holding a simple camera instance
   *
   */
  struct CameraComponent : public ISerializable
  {
    std::shared_ptr<ICamera> Camera;

    CameraComponent(std::shared_ptr<ICamera> camera)
      : Camera(std::move(camera))
    {
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json serializedMeshRendererComponent;
      if (Camera)
      {
        serializedMeshRendererComponent["Camera"] = Camera->Serialize();
      }
      else
      {
        serializedMeshRendererComponent["Camera"] = "";
      }

      return serializedMeshRendererComponent;
    }
  };
}