#include "dpch.h"
#include "Application/DwarfEngine.h"
#include "Launcher/ProjectLauncher.h"
#include "Editor/Editor.h"

namespace Dwarf
{

	DwarfEngine *DwarfEngine::s_Instance = nullptr;

	DwarfEngine::DwarfEngine()
	{
		s_Instance = this;
	}

	DwarfEngine::~DwarfEngine() = default;

	void DwarfEngine::Run()
	{
		bool shouldClose = false;
		while (!shouldClose)
		{
			auto launcher = Dwarf::CreateLauncher();
			std::filesystem::path projectPath = launcher->Run();
			delete launcher;

			if (!projectPath.empty())
			{
				std::cout << "Opening project at: <" << projectPath.string() << ">" << std::endl;

				auto editor = Dwarf::CreateEditor();
				shouldClose = !editor->Run(projectPath);
			}
			else
			{
				std::cout << "Not opening any project" << std::endl;
				shouldClose = true;
			}
		}
	}

	Ref<DwarfEngine> Create()
	{
		return CreateRef<DwarfEngine>();
	}
}
