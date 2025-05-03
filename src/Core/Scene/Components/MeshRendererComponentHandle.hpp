#pragma once

#include "Core/Scene/Components/SceneComponents.hpp"

namespace Dwarf
{
  class MeshRendererComponentHandle
  {
  private:
    entt::registry& mRegistry;
    entt::entity    mEntity;

  public:
    MeshRendererComponentHandle(entt::registry& reg, entt::entity ent)
      : mRegistry(reg)
      , mEntity(ent)
    {
    }

    [[nodiscard]] auto
    GetModelAsset() const -> const std::unique_ptr<IAssetReference>&
    {
      return mRegistry.get<MeshRendererComponent>(mEntity).ModelAsset;
    }

    void
    SetModelAsset(std::unique_ptr<IAssetReference>&& modelAsset)
    {
      mRegistry.patch<MeshRendererComponent>(
        mEntity,
        [modelAsset =
           std::move(modelAsset)](MeshRendererComponent& component) mutable
        { component.ModelAsset = std::move(modelAsset); });
    }

    [[nodiscard]] auto
    GetIdMeshBuffer() const -> const IMeshBuffer*
    {
      return mRegistry.get<MeshRendererComponent>(mEntity).IdMeshBuffer.get();
    }

    void
    SetIdMeshBuffer(std::unique_ptr<IMeshBuffer>&& idMeshBuffer)
    {
      mRegistry.get<MeshRendererComponent>(mEntity).IdMeshBuffer =
        std::move(idMeshBuffer);
      // mRegistry.patch<MeshRendererComponent>(
      //   mEntity,
      //   [idMeshBuffer =
      //      std::move(idMeshBuffer)](MeshRendererComponent& component) mutable
      //   { component.IdMeshBuffer = std::move(idMeshBuffer); });
    }

    [[nodiscard]] auto
    GetMaterialAssets() const
      -> const std::map<int, std::unique_ptr<IAssetReference>>&
    {
      return mRegistry.get<MeshRendererComponent>(mEntity).MaterialAssets;
    }

    void
    SetMaterialAsset(int index, std::unique_ptr<IAssetReference> materialAsset)
    {
      mRegistry.patch<MeshRendererComponent>(
        mEntity,
        [index, materialAsset = std::move(materialAsset)](
          MeshRendererComponent& component) mutable
        { component.MaterialAssets[index] = std::move(materialAsset); });
    }

    void
    ClearMaterialAssets()
    {
      mRegistry.patch<MeshRendererComponent>(
        mEntity,
        [](MeshRendererComponent& component)
        { component.MaterialAssets.clear(); });
    }

    [[nodiscard]] auto
    GetIsHidden() const -> bool
    {
      return mRegistry.get<MeshRendererComponent>(mEntity).IsHidden;
    }

    void
    SetIsHidden(bool isHidden)
    {
      mRegistry.patch<MeshRendererComponent>(
        mEntity,
        [isHidden](MeshRendererComponent& component)
        { component.IsHidden = isHidden; });
    }

    [[nodiscard]] auto
    GetCastShadow() const -> bool
    {
      return mRegistry.get<MeshRendererComponent>(mEntity).CastShadow;
    }

    void
    SetCastShadow(bool castShadow)
    {
      mRegistry.patch<MeshRendererComponent>(
        mEntity,
        [castShadow](MeshRendererComponent& component)
        { component.CastShadow = castShadow; });
    }
  };
}