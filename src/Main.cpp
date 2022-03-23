#include<iostream>
#include<string>
#include"engine/project launcher/ProjectLauncher.h"

int main() {
	int returnCode = 0;
	
	ProjectLauncher launcher = ProjectLauncher();
	returnCode = launcher.Run();
	
	return returnCode;
}