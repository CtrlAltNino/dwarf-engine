#pragma once

#include "../../utilities/dpch.h"

#ifdef _WIN32
#include<Windows.h>
#include<KnownFolders.h>
#include<ShlObj.h>
#include<filesystem>
#endif

/// @brief This class handles all file related tasks of the editor and project launcher.
class FileHandler{
	private:
		/// @brief Path to the documents directory.
		static inline std::string documentsPath;

		/// @brief Path to the project settings.
		static inline std::string projectSettingsPath;

		/// @brief Creates the platform depending path to the documents directory.
		/// @return Absolute path to the documents directory.
		static std::string CreateDocumentsFolderPath(){
			std::string defaultProjectPath;
			
			#if _WIN32
				static char str[128];
				{
					PWSTR path = NULL;
					HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);

					if (SUCCEEDED(hr)) {
						//newProjectPath = (const char*)path;
						size_t i;
						wcstombs_s(&i, str, (size_t)128, path, (size_t)127);
						defaultProjectPath = std::string((const char*)str);
					}

					CoTaskMemFree(path);
				}
			#endif

			#if __linux__
					std::string homeFolder = getenv("HOME");
					std::string userDirMap = readFile((homeFolder + "/.config/user-dirs.dirs").c_str());

					std::regex patternLine("XDG_DOCUMENTS.*");
					std::regex patternPath("\/.*(?=\")");
					std::smatch match;
					std::cout << std::regex_search(userDirMap, match, patternLine) << std::endl;
					std::string line = match[0];
					std::regex_search(line, match, patternPath);
					std::string path = match[0];
				
					defaultProjectPath = homeFolder + path;
			#endif

			return defaultProjectPath;
		}

		/// @brief Creates the path to the project settings.
		/// @return Absolute path to where the project settings file is located.
		static std::string CreateProjectSettingsPath(){
			std::string path = "";
			std::string subpath = "/Dwarf Engine/settings/";
			#if _WIN32
				path = GetDocumentsPath();
			#elif __linux__
				path = std::string(getenv("HOME")) + "/.config/";
			#endif

			return path + subpath;
		}
	public:
		/// @brief Initializes the file handler.
		static void InitFileHandler(){
			documentsPath = CreateDocumentsFolderPath();
			projectSettingsPath = CreateProjectSettingsPath();
		}

		/// @brief Returns the path to the document directory.
		/// @return An absolute path.
		static std::string GetDocumentsPath(){
			return documentsPath;
		}

		/// @brief Returns the path to the project settings file.
		/// @return An absolute path.
		static std::string GetProjectSettingsPath(){
			return projectSettingsPath;
		}

		/// @brief Checks if a file is present at a given path.
		/// @param filePath Absolute path to a file.
		/// @return True if file exists, false if not.
		static bool checkIfFileExists(const char* filePath) {
			std::ifstream fileStream(filePath, std::ios::in);
			return fileStream.is_open();
		}

		/// @brief Reads a file and returns the content.
		/// @param filePath Absolute path to a file.
		/// @return The content of the file.
		static std::string readFile(const char* filePath) {
			std::string content;
			std::ifstream fileStream(filePath, std::ios::in);

			if (!fileStream.is_open()) {
				std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
				return "";
			}

			std::string line = "";
			while (!fileStream.eof()) {
				std::getline(fileStream, line);
				content.append(line + "\n");
			}

			fileStream.close();
			return content;
		}

		/// @brief Writes a string to a file at a given path.
		/// @param filePath Path where to write the string to.
		/// @param content Content to write.
		static void writeToFile(const char* filePath, std::string content) {
			//opening a file in writing mode which is default.
			std::ofstream file;
			file.open(filePath);

			//COUT << "ENTER YOUR MESSAGE";

			//taking in put from user to write to test.dat file
			//string message;
			//cin >> mesage;
			file << content << std::endl;

			//We need to close every file which you open.
			file.close();
		}

		/// @brief Checks if a directory is present on the users disk.
		/// @param path Path to a directory.
		/// @return True if directory is present, false if not.
		static bool checkIfDirectoyExists(const char* path) {
			#if _WIN32
			DWORD ftyp = GetFileAttributesA(path);
			if (ftyp == INVALID_FILE_ATTRIBUTES)
				return false;  //something is wrong with your path!

			if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
				return true;   // this is a directory!
			#endif

			return false;    // this is not a directory!
		}

		/// @brief Checks if a directory is present on the users disk.
		/// @param path Path to a directory.
		/// @return True if directory is present, false if not.
		static bool checkIfDirectoyExists(std::string path) {
			return checkIfDirectoyExists(path.c_str());
		}

		/// @brief Creates a directory at a given path.
		/// @param path Path to a directory.
		static void createDirectoryC(const char* path) {
			#if _WIN32
				system((std::string("mkdir \"") + path + "\"").c_str());
			#elif __linux__
				system((std::string("mkdir -p \"") + path + "\"").c_str());
			#endif
		}

		/// @brief Creates a directory at a given path.
		/// @param path Path to a directory.
		static void createDirectoryS(std::string path) {
			createDirectoryC(path.c_str());
		}

		/// @brief Opens a path into a file browser.
		/// @param path Path to open.
		static void OpenPathInFileBrowser(std::string path){
			size_t pos;
			while ((pos = path.find('\\')) != std::string::npos) {
				path.replace(pos, 1, "/");
			}
			//std::cout << projectPath << std::endl;
			//system(std::string("explorer "" \"" + projectPath + "\" > nul").c_str());
			#if _WIN32
			wchar_t* commandStr = new wchar_t[4096];
			wchar_t* argStr = new wchar_t[4096];
			MultiByteToWideChar(CP_ACP, 0, "explore", -1, commandStr, 4096);
			MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, argStr, 4096);

			ShellExecute(NULL, commandStr, argStr, NULL, NULL, SW_SHOWNORMAL);
			#endif
		}
};

