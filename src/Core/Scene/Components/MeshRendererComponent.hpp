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

  /// @brief A component holding meshes and their corresponding materials, as
  /// well as other information on how to render these meshes.
  struct MeshRendererComponent : public ISerializable
  {
    std::unique_ptr<IAssetReference> ModelAsset = nullptr;
    std::unique_ptr<IMeshBuffer>     IdMeshBuffer = nullptr;

    /// @brief The materials with which the model is to be rendered. The list
    /// index of the materials corresponds to the material index of the
    /// submeshes.
    std::map<int, std::unique_ptr<IAssetReference>> MaterialAssets;

    bool IsHidden = false;
    bool CastShadow = true;

    MeshRendererComponent() = default;
    MeshRendererComponent(
      std::unique_ptr<IAssetReference>                modelAsset,
      std::map<int, std::unique_ptr<IAssetReference>> materials,
      bool                                            isHidden = false,
      bool                                            castShadow = true)
      : ModelAsset(std::move(modelAsset))
      , MaterialAssets(std::move(materials))
      , IsHidden(isHidden)
      , CastShadow(castShadow)
    {
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json serializedMeshRendererComponent;
      if (ModelAsset)
      {
        serializedMeshRendererComponent["Model"] =
          ModelAsset->GetUID().toString();
      }
      else
      {
        serializedMeshRendererComponent["Model"] = "";
      }

      int materialCount = 0;

      for (const auto& [index, material] : MaterialAssets)
      {
        serializedMeshRendererComponent["Materials"][std::to_string(index)] =
          material ? material->GetUID().toString() : "null";
      }

      serializedMeshRendererComponent["Hidden"] = IsHidden;

      return serializedMeshRendererComponent;
    }
  };
}