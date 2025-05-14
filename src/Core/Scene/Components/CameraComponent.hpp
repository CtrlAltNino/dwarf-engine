#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Rendering/LightTypes.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"
#include "Core/Scene/Camera/ICamera.hpp"
#include "Core/UUID.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"
#include <entt/entt.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <string>

namespace Dwarf
{
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