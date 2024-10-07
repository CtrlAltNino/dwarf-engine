#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/Inspector/EntityInspector/IEntityInspector.h"

namespace Dwarf
{
  class EntityInspector : public IEntityInspector
  {
  private:
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    std::shared_ptr<ILoadedScene>   m_LoadedScene;

    /// @brief Renders the components of an entity.
    /// @param entity Entity to render in the inspector.
    void
    RenderComponents(entt::entity entity);

    void
    BeginComponent(const char* componentHeader) const;

    void
    EndComponent() const;

    template<typename T>
    void
    RenderComponent(T& component);

  public:
    EntityInspector(std::shared_ptr<IAssetDatabase> assetDatabase,
                    std::shared_ptr<ILoadedScene>   loadedScene);
    ~EntityInspector() override = default;

    void
    Render(std::vector<entt::entity>& entities) override;
  };
}