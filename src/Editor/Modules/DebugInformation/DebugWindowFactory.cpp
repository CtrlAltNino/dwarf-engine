#include "DebugWindowFactory.h"

namespace Dwarf
{
  DebugWindowFactory::DebugWindowFactory(
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : mAssetDatabase(assetDatabase)
    , mInjectorFactory(
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
    return std::make_unique<DebugWindow>(mAssetDatabase);
  }

  std::unique_ptr<DebugWindow>
  DebugWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<DebugWindow>(mAssetDatabase, serializedModule);
  }
}