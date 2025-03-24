#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/Inspector/EntityInspector/IEntityInspector.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class EntityInspector : public IEntityInspector
  {
  private:
    std::shared_ptr<IDwarfLogger>   mLogger;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;
    std::shared_ptr<ILoadedScene>   mLoadedScene;

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
    EntityInspector(std::shared_ptr<IDwarfLogger>   logger,
                    std::shared_ptr<IAssetDatabase> assetDatabase,
                    std::shared_ptr<ILoadedScene>   loadedScene);
    ~EntityInspector() override = default;

    /**
     * @brief Renders the scene entity inspector
     *
     */
    void
    Render(const std::vector<entt::entity>& entities) override;
  };
}