#pragma once

class TempFileHelper
{
public:
  static void
  CreateTempFile(std::filesystem::path path, std::string content)
  {
    // Write content to the file
    std::ofstream tempFile(path);
    tempFile << content;
    tempFile.close();
  }

  static void
  DeleteTempFile(std::filesystem::path path)
  {
    std::filesystem::remove(path);
  }
};