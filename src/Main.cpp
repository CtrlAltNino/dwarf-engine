#define SDL_MAIN_HANDLED
#include<iostream>
#include<string>
//#include"engine/project launcher/ProjectLauncher.h"
#include"engine/project launcher/ProjectLauncherController.h"
#include"engine/editor/EditorController.h"

int main() {
	int returnCode = 0;
	
	//ProjectLauncher launcher = ProjectLauncher();
	//returnCode = launcher.Run();

	ProjectLauncherController launcher = ProjectLauncherController();
	launcher.Init();
	ProjectData chosenProject = launcher.Run();

	if(chosenProject.name != ""){
		EditorController editor(chosenProject);
		editor.RunLoop();
	}
	return returnCode;
}