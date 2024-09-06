#include "DebugWindowFactory.h"

namespace Dwarf
{
  DebugWindowFactory::DebugWindowFactory(
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : m_InjectorFactory(
        [&assetDatabase]()
        {
          return boost::di::make_injector(
            boost::di::bind<IAssetDatabase>.to(assetDatabase));
        })
  {
  }

  std::shared_ptr<DebugWindow>
  DebugWindowFactory::Create() const
  {
    return std::make_shared<DebugWindow>(
      m_InjectorFactory().create<DebugWindow>());
  }

  std::shared_ptr<DebugWindow>
  DebugWindowFactory::Create(SerializedModule serializedModule) const
  {
    auto injector = boost::di::make_injector(
      m_InjectorFactory(),
      boost::di::bind<SerializedModule>.to(serializedModule));

    return std::make_shared<DebugWindow>(injector.create<DebugWindow>());
  }
}