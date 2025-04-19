#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/Modules/Inspector/EntityInspector/IEntityInspector.hpp"
#include "Editor/Selection/IEditorSelection.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class EntityInspector : public IEntityInspector
  {
  private:
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<ILoadedScene>     mLoadedScene;
    std::shared_ptr<IEditorSelection> mEditorSelection;

    /// @brief Renders the components of an entity.
    /// @param entity Entity to render in the inspector.
    void
    RenderComponents(entt::entity entity);

    static void
    BeginComponent(const char* componentHeader);

    static void
    EndComponent();

    template<typename T>
    void
    RenderComponent(T& component);

  public:
    EntityInspector(std::shared_ptr<IDwarfLogger>     logger,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<ILoadedScene>     loadedScene,
                    std::shared_ptr<IEditorSelection> editorSelection);
    ~EntityInspector() override = default;

    /**
     * @brief Renders the scene entity inspector
     *
     */
    void
    Render() override;
  };
}