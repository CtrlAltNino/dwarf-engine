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

  std::unique_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create() const
  {
    return m_InjectorFactory().create<std::unique_ptr<PerformanceWindow>>();
  }

  std::unique_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create(SerializedModule serializedModule) const
  {
    auto injector = boost::di::make_injector(
      m_InjectorFactory(),
      boost::di::bind<SerializedModule>.to(serializedModule));

    return injector.create<std::unique_ptr<PerformanceWindow>>();
  }
} // namespace Dwarf