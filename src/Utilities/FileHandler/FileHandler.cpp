#include "Utilities/FileHandler/FileHandler.h"

#include <sago/platform_folders.h>

#ifdef _WIN32
#include <Windows.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <shlwapi.h>
#include <objbase.h>
#endif

#ifdef __linux__
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace Dwarf
{
  FileHandler::FileHandler(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
    , m_DocumentsPath(CreateDocumentsFolderPath())
    , m_EngineSettingsPath(CreateEngineSettingsPath())
  {
  }

  /// @brief Creates the platform depending path to the documents directory.
  /// @return Absolute path to the documents directory.
  std::filesystem::path
  FileHandler::CreateDocumentsFolderPath()
  {
    m_Logger->LogDebug(Log("CreateDocumentsFolderPath", "FileHandler"));
    std::filesystem::path defaultProjectPath;

#if _WIN32
    static char str[128];
    {
      PWSTR   path = NULL;
      HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);

      if (SUCCEEDED(hr))
      {
        // newProjectPath = (const char*)path;
        size_t i;
        wcstombs_s(&i, str, (size_t)128, path, (size_t)127);
        defaultProjectPath = std::string((const char*)str);
      }

      CoTaskMemFree(path);
    }
#endif

#if __linux__
    std::string homeFolder = getenv("HOME");
    std::string userDirMap =
      ReadFile((homeFolder + "/.config/user-dirs.dirs").c_str());

    std::regex  patternLine("XDG_DOCUMENTS.*");
    std::regex  patternPath("/.*(?=\")");
    std::smatch match;
    std::regex_search(userDirMap, match, patternLine);
    std::string line = match[0];
    std::regex_search(line, match, patternPath);
    std::string path = match[0];

    defaultProjectPath = homeFolder + path;
#endif

    return defaultProjectPath;
  }

  /// @brief Creates the path to the project settings.
  /// @return Absolute path to where the project settings file is located.
  std::filesystem::path
  FileHandler::CreateEngineSettingsPath()
  {
    std::filesystem::path path = "";
    std::filesystem::path subpath = "Dwarf Engine/settings";
#if _WIN32
    path = GetDocumentsPath();
#elif __linux__
    path = std::string(getenv("HOME")) + "/.config/";
#endif

    return path / subpath;
  }

  /// @brief Returns the path to the document directory.
  /// @return An absolute path.
  std::filesystem::path
  FileHandler::GetDocumentsPath()
  {
    m_Logger->LogDebug(Log("GetDocumentsPath", "FileHandler"));
    return m_DocumentsPath;
  }

  /// @brief Returns the path to the project settings file.
  /// @return An absolute path.
  std::filesystem::path
  FileHandler::GetEngineSettingsPath()
  {
    m_Logger->LogDebug(Log("GetEngineSettingsPath", "FileHandler"));
    return m_EngineSettingsPath;
  }

  /// @brief Checks if a file is present at a given path.
  /// @param filePath Absolute path to a file.
  /// @return True if file exists, false if not.
  bool
  FileHandler::FileExists(std::filesystem::path const& filePath)
  {
    m_Logger->LogDebug(Log("FileExists", "FileHandler"));
    std::filesystem::path copy = filePath;
    std::ifstream         fileStream(copy.make_preferred(), std::ios::in);
    return fileStream.is_open();
  }

  /// @brief Reads a file and returns the content.
  /// @param filePath Absolute path to a file.
  /// @return The content of the file.
  std::string
  FileHandler::ReadFile(std::filesystem::path const& filePath)
  {
    m_Logger->LogDebug(Log("ReadFile", "FileHandler"));
    std::string   content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
      std::cerr << "Could not read file " << filePath
                << ". File does not exist." << std::endl;
      return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
      std::getline(fileStream, line);
      content.append(line + "\n");
    }

    fileStream.close();
    return content;
  }

  /// @brief Writes a string to a file at a given path.
  /// @param filePath Path where to write the string to.
  /// @param content Content to write.
  void
  FileHandler::WriteToFile(std::filesystem::path const& filePath,
                           std::string_view             content)
  {
    m_Logger->LogDebug(Log("WriteToFile", "FileHandler"));
    // opening a file in writing mode which is default.
    std::ofstream file;
    file.open(filePath);

    // COUT << "ENTER YOUR MESSAGE";

    // taking in put from user to write to test.dat file
    // string message;
    // cin >> mesage;
    file << content << std::endl;

    // We need to close every file which you open.
    file.close();
  }

  /// @brief Checks if a directory is present on the users disk.
  /// @param path Path to a directory.
  /// @return True if directory is present, false if not.
  bool
  FileHandler::DirectoryExists(std::filesystem::path const& path)
  {
    m_Logger->LogDebug(Log("DirectoryExists", "FileHandler"));
    return std::filesystem::exists(path);
  }

  /// @brief Creates a directory at a given path.
  /// @param path Path to a directory.
  void
  FileHandler::CreateDirectoryAt(std::filesystem::path const& path)
  {
    m_Logger->LogDebug(Log("CreateDirectoryAt", "FileHandler"));
    std::filesystem::create_directories(path);
  }

  /// @brief Opens a path into a file browser.
  /// @param path Path to open.
  void
  FileHandler::OpenPathInFileBrowser(std::filesystem::path const& path)
  {
    m_Logger->LogDebug(Log("OpenPathInFileBrowser", "FileHandler"));
#if _WIN32
    wchar_t* commandStr = new wchar_t[4096];
    wchar_t* argStr = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, "explore", -1, commandStr, 4096);
    MultiByteToWideChar(CP_ACP, 0, path.string().c_str(), -1, argStr, 4096);

    ShellExecute(NULL, commandStr, argStr, NULL, NULL, SW_SHOWNORMAL);
    delete[] (commandStr);
    delete[] (argStr);
#endif
#if __linux__
    pid_t pid = fork();

    if (pid == 0)
    { // Child process
      execlp("xdg-open", "xdg-open", path.string().c_str(), nullptr);
    }
    else if (pid > 0)
    {                           // Parent process
      waitpid(pid, nullptr, 0); // Wait for the child process to complete
    }
    else
    {
      // Fork failed
      perror("fork failed");
    }
#endif
  }

  void
  FileHandler::LaunchFile(std::filesystem::path path)
  {
    m_Logger->LogDebug(Log("LaunchFile", "FileHandler"));
    // Start-Process -FilePath "custom.frag"
#if _WIN32
    wchar_t* commandStr = new wchar_t[4096];
    wchar_t* argStr = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, "open", -1, commandStr, 4096);
    MultiByteToWideChar(
      CP_ACP, 0, path.make_preferred().string().c_str(), -1, argStr, 4096);

    ShellExecute(NULL, NULL, argStr, NULL, NULL, SW_SHOWNORMAL);
    delete[] (commandStr);
    delete[] (argStr);
    // system(path.string().c_str());
#endif
#if __linux__
    pid_t pid = fork();

    if (pid == 0)
    { // Child process
      execlp("xdg-open", "xdg-open", path.string().c_str(), nullptr);
    }
    else if (pid > 0)
    {                           // Parent process
      waitpid(pid, nullptr, 0); // Wait for the child process to complete
    }
    else
    {
      // Fork failed
      perror("fork failed");
    }
#endif
  }

  void
  FileHandler::Copy(std::filesystem::path const& from,
                    std::filesystem::path const& to)
  {
    m_Logger->LogDebug(Log("Copy", "FileHandler"));
    std::filesystem::copy(from, to);
  }

  void
  FileHandler::Rename(std::filesystem::path const& oldPath,
                      std::filesystem::path const& newPath)
  {
    m_Logger->LogDebug(Log("Rename", "FileHandler"));
    std::filesystem::rename(oldPath, newPath);
  }

  void
  FileHandler::Duplicate(std::filesystem::path const& path)
  {
    m_Logger->LogDebug(Log("Duplicate", "FileHandler"));
    std::filesystem::path from = path;
    std::filesystem::path to = path;
    to = to.replace_filename(
      to.replace_extension("").filename().concat(" copy").concat(
        from.extension().string()));

    std::filesystem::copy(from, to);
  }

  void
  FileHandler::Delete(std::filesystem::path const& path)
  {
    m_Logger->LogDebug(Log("Delete", "FileHandler"));
    std::filesystem::remove(path);
  }
}