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
    SelectEntity(const Entity& entity) = 0;
    virtual void
    SelectAsset(const std::filesystem::path& assetPath) = 0;
    virtual void
    AddEntityToSelection(const Entity& entity) = 0;
    virtual void
    ClearEntitySelection() = 0;
    virtual void
    ClearAssetSelection() = 0;
    virtual void
    RemoveEntityFromSelection(const Entity& entity) = 0;
    virtual bool
    IsEntitySelected(const Entity& entity) = 0;
    virtual bool
    IsAssetSelected(const std::filesystem::path& assetPath) = 0;
    virtual std::vector<Entity>&
    GetSelectedEntities() = 0;
    virtual const std::filesystem::path&
    GetAssetPath() const = 0;
    virtual CURRENT_SELECTION_TYPE
    GetSelectionType() const = 0;
  };
}