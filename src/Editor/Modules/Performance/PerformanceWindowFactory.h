#pragma once

#include "Editor/Modules/Performance/IPerformanceWindowFactory.h"

namespace Dwarf
{
  class PerformanceWindowFactory : public IPerformanceWindowFactory
  {
  private:
    std::function<boost::di::injector<std::unique_ptr<PerformanceWindow>>()>
      m_InjectorFactory;

  public:
    BOOST_DI_INJECT(PerformanceWindowFactory,
                    std::shared_ptr<IEditorStats> editorStats);

    ~PerformanceWindowFactory() override = default;
    std::unique_ptr<PerformanceWindow>
    Create() const override;

    std::unique_ptr<PerformanceWindow>
    Create(SerializedModule serializedModule) const override;
  };
}