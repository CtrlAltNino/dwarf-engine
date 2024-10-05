#include "PerformanceWindowFactory.h"

namespace Dwarf
{
  PerformanceWindowFactory::PerformanceWindowFactory(
    std::shared_ptr<IEditorStats> editorStats)
    : m_EditorStats(editorStats)
  {
  }

  std::unique_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create() const
  {
    return std::make_unique<PerformanceWindow>(m_EditorStats);
  }

  std::unique_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<PerformanceWindow>(serializedModule, m_EditorStats);
  }
} // namespace Dwarf