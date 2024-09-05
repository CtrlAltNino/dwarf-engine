#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/Modules/Inspector/EntityInspector/IEntityInspector.h"

namespace Dwarf
{
  class EntityInspector : public IEntityInspector
  {
  private:
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    /// @brief Renders the components of an entity.
    /// @param entity Entity to render in the inspector.
    void
    RenderComponents(Entity entity);

    void
    BeginComponent(const char* componentHeader) const;

    void
    EndComponent() const;

    template<typename T>
    void
    RenderComponent(T& component);

  public:
    EntityInspector(std::shared_ptr<IAssetDatabase> assetDatabase);
    ~EntityInspector() override = default;

    void
    Render(std::vector<Entity>& entities) override;
  };
}