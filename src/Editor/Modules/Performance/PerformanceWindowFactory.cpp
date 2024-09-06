#include "PerformanceWindowFactory.h"

namespace Dwarf
{
  PerformanceWindowFactory::PerformanceWindowFactory(
    std::shared_ptr<IEditorStats> editorStats)
    : m_InjectorFactory(
        [&editorStats]()
        {
          return boost::di::make_injector(
            boost::di::bind<IEditorStats>.to(editorStats));
        })
  {
  }

  std::shared_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create() const
  {
    return std::make_shared<PerformanceWindow>(
      m_InjectorFactory().create<PerformanceWindow>());
  }

  std::shared_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create(SerializedModule serializedModule) const
  {
    auto injector = boost::di::make_injector(
      m_InjectorFactory(),
      boost::di::bind<SerializedModule>.to(serializedModule));

    return std::make_shared<PerformanceWindow>(
      injector.create<PerformanceWindow>());
  }
} // namespace Dwarf