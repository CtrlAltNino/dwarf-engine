#include "Editor/Selection/EditorSelection.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "pch.h"

namespace Dwarf
{
  EditorSelection::EditorSelection(std::shared_ptr<ILoadedScene> loadedScene)
    : mLoadedScene(loadedScene)
  {
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
    mSelectedEntities.erase(
      std::find(mSelectedEntities.begin(), mSelectedEntities.end(), entity));
    if (mSelectedEntities.empty())
    {
      mSelectionType = mSelectedAsset ? CURRENT_SELECTION_TYPE::ASSET
                                      : CURRENT_SELECTION_TYPE::NONE;
    }
  }

  bool
  EditorSelection::IsEntitySelected(const entt::entity& entity)
  {
    return std::find(mSelectedEntities.begin(),
                     mSelectedEntities.end(),
                     entity) != mSelectedEntities.end();
  }

  bool
  EditorSelection::IsAssetSelected(const std::filesystem::path& assetPath)
  {
    return mSelectedAsset && (mSelectedAsset->GetPath() == assetPath);
  }

  std::string
  EditorSelection::GetTreeIndex(const entt::entity& entity) const
  {
    std::string  index = "";
    entt::entity cursor = entity;

    while (cursor != mLoadedScene->GetScene().GetRootEntity().GetHandle())
    {
      Entity ent(cursor, mLoadedScene->GetScene().GetRegistry());
      index = std::format("{}{}", std::to_string(ent.GetChildIndex()), index);
      cursor = ent.GetParent();
    }

    return index;
  }

  std::vector<entt::entity>&
  EditorSelection::GetSelectedEntities()
  {
    return mSelectedEntities;
  }

  CURRENT_SELECTION_TYPE
  EditorSelection::GetSelectionType() const
  {
    return mSelectionType;
  }

  IAssetReference&
  EditorSelection::GetSelectedAsset() const
  {
    return *mSelectedAsset;
  }
}