#pragma once

#include "Editor/Modules/Performance/IPerformanceWindowFactory.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class PerformanceWindowFactory : public IPerformanceWindowFactory
  {
  private:
    std::shared_ptr<IEditorStats> m_EditorStats;

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