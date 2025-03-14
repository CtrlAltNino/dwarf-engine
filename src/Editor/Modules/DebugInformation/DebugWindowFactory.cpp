#include "DebugWindowFactory.h"

namespace Dwarf
{
  DebugWindowFactory::DebugWindowFactory(
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : m_AssetDatabase(assetDatabase)
    , m_InjectorFactory(
        [](std::shared_ptr<IAssetDatabase> assetDatabase)
        {
          return boost::di::make_injector(
            boost::di::bind<IAssetDatabase>.to(assetDatabase));
        })
  {
  }

  std::unique_ptr<DebugWindow>
  DebugWindowFactory::Create() const
  {
    return std::make_unique<DebugWindow>(m_AssetDatabase);
  }

  std::unique_ptr<DebugWindow>
  DebugWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<DebugWindow>(m_AssetDatabase, serializedModule);
  }
}