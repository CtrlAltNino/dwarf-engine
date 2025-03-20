#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/Modules/DebugInformation/IDebugWindowFactory.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class DebugWindowFactory : public IDebugWindowFactory
  {
  private:
    std::shared_ptr<IAssetDatabase> mAssetDatabase;
    std::function<boost::di::injector<std::unique_ptr<DebugWindow>>(
      std::shared_ptr<IAssetDatabase>)>
      mInjectorFactory;

  public:
    BOOST_DI_INJECT(DebugWindowFactory,
                    std::shared_ptr<IAssetDatabase> assetDatabase);
    ~DebugWindowFactory() override = default;
    std::unique_ptr<DebugWindow>
    Create() const override;

    std::unique_ptr<DebugWindow>
    Create(SerializedModule serializedModule) const override;
  };
}