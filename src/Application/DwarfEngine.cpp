#include "dpch.h"
#include "Application/DwarfEngine.h"
#include "Launcher/ProjectLauncher.h"
#include "Editor/Editor.h"

namespace Dwarf{

	DwarfEngine* DwarfEngine::s_Instance = nullptr;

	DwarfEngine::DwarfEngine() {
		s_Instance = this;
	}

	DwarfEngine::~DwarfEngine(){

	}

	void DwarfEngine::Run() {
		auto launcher = Dwarf::CreateLauncher();
		std::filesystem::path projectPath = launcher->Run();
		delete(launcher);

		if(!projectPath.empty()){
			std::cout << "Opening project at: <" << projectPath.string() << ">" << std::endl;

			auto editor = Dwarf::CreateEditor();
			editor->Run(projectPath);

			//delete(editor);
		}else{
			std::cout << "Not opening any project" << std::endl;
		}
	}

	DwarfEngine* Create(){
		return new DwarfEngine();
	}
}
