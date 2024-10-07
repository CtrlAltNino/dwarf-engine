#pragma once
#include "Core/Scene/Entity/Entity.h"

namespace Dwarf
{
  enum class CURRENT_SELECTION_TYPE
  {
    NONE,
    ASSET,
    ENTITY
  };

  class IEditorSelection
  {
  public:
    virtual ~IEditorSelection() = default;

    virtual void
    SelectEntity(const entt::entity& entity) = 0;

    virtual void
    SelectAsset(std::unique_ptr<IAssetReference> assetPath) = 0;

    virtual void
    AddEntityToSelection(const entt::entity& entity) = 0;

    virtual void
    ClearEntitySelection() = 0;

    virtual void
    ClearAssetSelection() = 0;

    virtual void
    RemoveEntityFromSelection(const entt::entity& entity) = 0;

    virtual bool
    IsEntitySelected(const entt::entity& entity) = 0;

    virtual bool
    IsAssetSelected(const std::filesystem::path& assetPath) = 0;

    virtual std::vector<entt::entity>&
    GetSelectedEntities() = 0;

    virtual IAssetReference&
    GetSelectedAsset() const = 0;

    virtual CURRENT_SELECTION_TYPE
    GetSelectionType() const = 0;
  };
}