#pragma once

#include "Editor/Modules/DebugInformation/IDebugWindowFactory.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class DebugWindowFactory : public IDebugWindowFactory
  {
  private:
    std::function<boost::di::injector<DebugWindow>()> m_InjectorFactory;

  public:
    BOOST_DI_INJECT(DebugWindowFactory,
                    std::shared_ptr<IAssetDatabase> assetDatabase);
    ~DebugWindowFactory() override = default;
    std::shared_ptr<DebugWindow>
    Create() const override;

    std::shared_ptr<DebugWindow>
    Create(SerializedModule serializedModule) const override;
  };
}