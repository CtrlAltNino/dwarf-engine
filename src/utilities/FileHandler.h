#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<fstream>

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
};

