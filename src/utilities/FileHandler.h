#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<fstream>

#ifdef _WIN32
	#include<Windows.h>
#endif

class FileHandler{
public:
	static bool checkIfFileExists(const char* filePath) {
		std::ifstream fileStream(filePath, std::ios::in);
		return fileStream.is_open();
	}

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

	static bool checkIfDirectoyExists(std::string path) {
		return checkIfDirectoyExists(path.c_str());
	}

	static void createDirectoryC(const char* path) {
		#if _WIN32
		WCHAR    str[sizeof(path)];
		MultiByteToWideChar(0, 0, path, 5, str, 6);
		LPCWSTR cstr = str;
		#endif

		//CreateDirectory(cstr, NULL);
		system((std::string("mkdir \"") + path + "\"").c_str());
	}

	static void createDirectoryS(std::string path) {
		createDirectoryC(path.c_str());
	}
};

