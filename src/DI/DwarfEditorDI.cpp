#include "DwarfEditorDI.h"
#include "Editor/Editor.h"
#include "Editor/EditorView/EditorView.h"
#include "Editor/IEditor.h"
#include "Project/ProjectTypes.h"

namespace Dwarf
{
  DwarfEditorDI::DwarfEditorDI(std::filesystem::path projectPath)
    : m_Injector(boost::di::make_injector(
        boost::di::bind<ProjectPath>.to(ProjectPath(projectPath)),
        boost::di::bind<IEditor>.to<Editor>(),
        boost::di::bind<IEditorView>.to<EditorView>()))
  {
  }
  const boost::di::injector<IEditor>&
  DwarfEditorDI::Get() const
  {
    return m_Injector;
  }

}