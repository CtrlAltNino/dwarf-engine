#include "Core/Asset/AssetReference/IAssetReference.h"
#include "pch.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Selection/EditorSelection.h"

namespace Dwarf
{
  EditorSelection::EditorSelection(
    std::shared_ptr<ILoadedScene>                               loadedScene,
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase)
    : m_LoadedScene(loadedScene)
  //, m_AssetDatabase(assetDatabase)
  {
  }

  void
  EditorSelection::SelectEntity(const Entity& entity)
  {
    ClearEntitySelection();
    AddEntityToSelection(entity);
    m_SelectionType = CURRENT_SELECTION_TYPE::ENTITY;
  }

  void
  EditorSelection::SelectAsset(std::unique_ptr<IAssetReference> asset)
  {
    m_SelectedAsset = std::move(asset);
    m_SelectionType = CURRENT_SELECTION_TYPE::ASSET;
  }

  void
  EditorSelection::AddEntityToSelection(const Entity& entity)
  {
    std::string index = GetTreeIndex(entity);

    auto        cursor = m_SelectedEntities.begin();
    std::string cursorIndex;

    while ((cursor != m_SelectedEntities.end()) &&
           ((cursorIndex = GetTreeIndex(*cursor)) < index))
    {
      cursor++;
    }

    if (cursor == m_SelectedEntities.end())
    {
      m_SelectedEntities.push_back(entity);
    }
    else
    {
      m_SelectedEntities.insert(cursor, entity);
    }

    m_SelectionType = CURRENT_SELECTION_TYPE::ENTITY;
  }

  void
  EditorSelection::ClearEntitySelection()
  {
    m_SelectedEntities.clear();
    m_SelectionType = m_SelectedAsset ? CURRENT_SELECTION_TYPE::ASSET
                                      : CURRENT_SELECTION_TYPE::NONE;
  }

  void
  EditorSelection::ClearAssetSelection()
  {
    m_SelectedAsset.reset();
    m_SelectionType = m_SelectedEntities.empty()
                        ? CURRENT_SELECTION_TYPE::NONE
                        : CURRENT_SELECTION_TYPE::ENTITY;
  }

  void
  EditorSelection::RemoveEntityFromSelection(const Entity& entity)
  {
    m_SelectedEntities.erase(
      std::find(m_SelectedEntities.begin(), m_SelectedEntities.end(), entity));
    if (m_SelectedEntities.empty())
    {
      m_SelectionType = m_SelectedAsset ? CURRENT_SELECTION_TYPE::ASSET
                                        : CURRENT_SELECTION_TYPE::NONE;
    }
  }

  bool
  EditorSelection::IsEntitySelected(const Entity& entity)
  {
    return std::find(m_SelectedEntities.begin(),
                     m_SelectedEntities.end(),
                     entity) != m_SelectedEntities.end();
  }

  bool
  EditorSelection::IsAssetSelected(const std::filesystem::path& assetPath)
  {
    return m_SelectedAsset && (m_SelectedAsset->GetPath() == assetPath);
  }

  std::string
  EditorSelection::GetTreeIndex(const Entity& entity) const
  {
    std::string index = "";
    Entity      cursor = entity;

    while (cursor.GetHandle() !=
           m_LoadedScene->GetScene().GetRootEntity().GetHandle())
    {
      index =
        std::format("{}{}", std::to_string(cursor.GetChildIndex()), index);
      cursor =
        Entity(cursor.GetParent(), m_LoadedScene->GetScene().GetRegistry());
    }

    return index;
  }

  std::vector<Entity>&
  EditorSelection::GetSelectedEntities()
  {
    return m_SelectedEntities;
  }

  CURRENT_SELECTION_TYPE
  EditorSelection::GetSelectionType() const
  {
    return m_SelectionType;
  }

  IAssetReference&
  EditorSelection::GetSelectedAsset() const
  {
    return *m_SelectedAsset;
  }
}