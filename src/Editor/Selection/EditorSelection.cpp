#include "pch.hpp"

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/Selection/EditorSelection.hpp"

namespace Dwarf
{
  EditorSelection::EditorSelection(std::shared_ptr<IDwarfLogger> logger,
                                   std::shared_ptr<ILoadedScene> loadedScene)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
  {
    mLogger->LogDebug(Log("EditorSelection created.", "EditorSelection"));
  }

  EditorSelection::~EditorSelection()
  {
    mLogger->LogDebug(Log("EditorSelection destroyed.", "EditorSelection"));
  }

  void
  EditorSelection::SelectEntity(const entt::entity& entity)
  {
    ClearEntitySelection();
    AddEntityToSelection(entity);
    mSelectionType = CURRENT_SELECTION_TYPE::ENTITY;
  }

  void
  EditorSelection::SelectAsset(std::unique_ptr<IAssetReference> asset)
  {
    mSelectedAsset = std::move(asset);
    mSelectionType = CURRENT_SELECTION_TYPE::ASSET;
  }

  void
  EditorSelection::AddEntityToSelection(const entt::entity& entity)
  {
    std::string index = GetTreeIndex(entity);

    auto        cursor = mSelectedEntities.begin();
    std::string cursorIndex;

    while ((cursor != mSelectedEntities.end()) &&
           ((cursorIndex = GetTreeIndex(*cursor)) < index))
    {
      cursor++;
    }

    if (cursor == mSelectedEntities.end())
    {
      mSelectedEntities.push_back(entity);
    }
    else
    {
      mSelectedEntities.insert(cursor, entity);
    }

    mSelectionType = CURRENT_SELECTION_TYPE::ENTITY;
  }

  void
  EditorSelection::ClearEntitySelection()
  {
    mSelectedEntities.clear();
    mSelectionType = mSelectedAsset ? CURRENT_SELECTION_TYPE::ASSET
                                    : CURRENT_SELECTION_TYPE::NONE;
  }

  void
  EditorSelection::ClearAssetSelection()
  {
    mSelectedAsset.reset();
    mSelectionType = mSelectedEntities.empty() ? CURRENT_SELECTION_TYPE::NONE
                                               : CURRENT_SELECTION_TYPE::ENTITY;
  }

  void
  EditorSelection::RemoveEntityFromSelection(const entt::entity& entity)
  {
    mSelectedEntities.erase(std::ranges::find(mSelectedEntities, entity));
    if (mSelectedEntities.empty())
    {
      mSelectionType = mSelectedAsset ? CURRENT_SELECTION_TYPE::ASSET
                                      : CURRENT_SELECTION_TYPE::NONE;
    }
  }

  auto
  EditorSelection::IsEntitySelected(const entt::entity& entity) -> bool
  {
    return std::ranges::find(mSelectedEntities, entity) !=
           mSelectedEntities.end();
  }

  auto
  EditorSelection::IsAssetSelected(const std::filesystem::path& assetPath)
    -> bool
  {
    return mSelectedAsset && mSelectedAsset->IsValid() &&
           (mSelectedAsset->GetPath() == assetPath);
  }

  auto
  EditorSelection::GetTreeIndex(const entt::entity& entity) const -> std::string
  {
    std::string  index;
    entt::entity cursor = entity;

    while (cursor != mLoadedScene->GetScene().GetRootEntity().GetHandle())
    {
      Entity ent(cursor, mLoadedScene->GetScene().GetRegistry());
      index = std::format("{}{}", std::to_string(ent.GetChildIndex()), index);
      cursor = ent.GetParent();
    }

    return index;
  }

  auto
  EditorSelection::GetSelectedEntities() -> std::vector<entt::entity>&
  {
    return mSelectedEntities;
  }

  auto
  EditorSelection::GetSelectionType() const -> CURRENT_SELECTION_TYPE
  {
    return mSelectionType;
  }

  auto
  EditorSelection::GetSelectedAsset() -> std::unique_ptr<IAssetReference>&
  {
    return mSelectedAsset;
  }
}