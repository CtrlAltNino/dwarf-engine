#include "DwarfEditorDI.h"
#include "Editor/Editor.h"
#include "Editor/EditorView.h"
#include "Editor/IEditor.h"
#include "Project/ProjectTypes.h"

namespace Dwarf
{
  boost::di::injector<Editor>
  DwarfEditorDI::CreateInjector(std::filesystem::path projectPath)
  {
    return boost::di::make_injector(
      boost::di::bind<ProjectPath>.to(ProjectPath(projectPath)),
      boost::di::bind<IEditor>.to<Editor>(),
      boost::di::bind<IEditorView>.to<EditorView>());
  }

}