#define SDL_MAIN_HANDLED
#include "Launcher/ProjectLauncherController.h"
#include "Editor/EditorController.h"

int main() {
	int returnCode = 0;

	Dwarf::ProjectLauncherController launcher = Dwarf::ProjectLauncherController();
	launcher.Init();
	std::filesystem::path projectPath = launcher.Run();

	Dwarf::EditorController editor(projectPath);
	editor.RunLoop();
	
	return returnCode;
}