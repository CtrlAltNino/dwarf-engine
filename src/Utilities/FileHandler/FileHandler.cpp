#include "pch.h"

#include "Utilities/FileHandler/FileHandler.h"
#include <filesystem>
#include <sago/platform_folders.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32
#include <KnownFolders.h>
#include <ShlObj.h>
#include <objbase.h>
#include <shlwapi.h>
#endif

#ifdef __linux__
#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace Dwarf
{
  FileHandler::FileHandler(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
    , mDocumentsPath(CreateDocumentsFolderPath())
    , mEngineSettingsPath(CreateEngineSettingsPath())
  {
  }

  /// @brief Creates the platform depending path to the documents directory.
  /// @return Absolute path to the documents directory.
  auto
  FileHandler::CreateDocumentsFolderPath() const -> std::filesystem::path
  {
    mLogger->LogDebug(Log("CreateDocumentsFolderPath", "FileHandler"));
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
  auto
  FileHandler::CreateEngineSettingsPath() const -> std::filesystem::path
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
  auto
  FileHandler::GetDocumentsPath() const -> std::filesystem::path
  {
    mLogger->LogDebug(Log("GetDocumentsPath", "FileHandler"));
    return mDocumentsPath;
  }

  /// @brief Returns the path to the project settings file.
  /// @return An absolute path.
  auto
  FileHandler::GetEngineSettingsPath() const -> std::filesystem::path
  {
    mLogger->LogDebug(Log("GetEngineSettingsPath", "FileHandler"));
    return mEngineSettingsPath;
  }

  /// @brief Checks if a file is present at a given path.
  /// @param filePath Absolute path to a file.
  /// @return True if file exists, false if not.
  auto
  FileHandler::FileExists(const std::filesystem::path& filePath) const -> bool
  {
    mLogger->LogDebug(Log("FileExists", "FileHandler"));
    return std::filesystem::exists(filePath);
  }

  /// @brief Reads a file and returns the content.
  /// @param filePath Absolute path to a file.
  /// @return The content of the file.
  auto
  FileHandler::ReadFile(std::filesystem::path const& filePath) const
    -> std::string
  {
#ifdef _WIN32
    HANDLE hFile =
      CreateFileA(filePath.string().c_str(),
                  GENERIC_READ,
                  FILE_SHARE_READ,
                  NULL,
                  OPEN_EXISTING,
                  FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN,
                  NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
      mLogger->LogError(Log(fmt::format("Error opening file: {} ({})",
                                        filePath.string(),
                                        GetLastError()),
                            "FileHandler"));
      return {};
    }

    // Get file size
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize))
    {
      mLogger->LogError(
        Log(fmt::format("Error getting file size: {}", GetLastError()),
            "FileHandler"));
      CloseHandle(hFile);
      return {};
    }

    constexpr size_t sectorSize = 4096; // Adjust based on system
    size_t           alignedSize =
      (fileSize.QuadPart + sectorSize - 1) & ~(sectorSize - 1);

    // Allocate sector-aligned memory
    void* rawBuffer = _aligned_malloc(alignedSize, sectorSize);
    if (rawBuffer == nullptr)
    {
      mLogger->LogError(
        Log(fmt::format("Memory allocation failed while reading file"),
            "FileHandler"));
      CloseHandle(hFile);
      return {};
    }

    DWORD bytesRead = 0;
    if (!::ReadFile(
          hFile, rawBuffer, static_cast<DWORD>(alignedSize), &bytesRead, NULL))
    {
      mLogger->LogError(Log(
        fmt::format("Error reading file: {}", GetLastError()), "FileHandler"));
      _aligned_free(rawBuffer);
      CloseHandle(hFile);
      return {};
    }

    CloseHandle(hFile);

    // Copy only the actual file size
    /*std::vector<unsigned char> buffer(static_cast<unsigned char*>(rawBuffer),
                                      static_cast<unsigned char*>(rawBuffer) +
                                        fileSize.QuadPart);*/
    std::string content(static_cast<char*>(rawBuffer), fileSize.QuadPart);
    _aligned_free(rawBuffer);

    return content;
#elif __linux__
    int fd = open(filePath.string().c_str(), O_RDONLY);
    if (fd == -1)
    {
      perror("open failed");
      return {};
    }

    // Get file size
    off_t fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    std::vector<char> buffer(fileSize);
    ssize_t           bytesRead = read(fd, buffer.data(), fileSize);
    close(fd);

    if (bytesRead != fileSize)
    {
      mLogger->LogError(
        Log(fmt::format("Error reading file: {}", filePath.string()),
            "FileHandler"));
      return {};
    }

    return std::string(buffer.data(), fileSize);
#endif
  }

  /// @brief Writes a string to a file at a given path.
  /// @param filePath Path where to write the string to.
  /// @param content Content to write.
  void
  FileHandler::WriteToFile(std::filesystem::path const& filePath,
                           std::string_view             content) const
  {
    // opening a file in writing mode which is default.
    std::ofstream file;
    file.open(filePath);

    // COUT << "ENTER YOUR MESSAGE";

    // taking in put from user to write to test.dat file
    // string message;
    // cin >> mesage;
    file << content << "\n";

    // We need to close every file which you open.
    file.close();
  }

  /// @brief Checks if a directory is present on the users disk.
  /// @param path Path to a directory.
  /// @return True if directory is present, false if not.
  auto
  FileHandler::DirectoryExists(std::filesystem::path const& path) const -> bool
  {
    mLogger->LogDebug(Log("DirectoryExists", "FileHandler"));
    return std::filesystem::exists(path);
  }

  /// @brief Creates a directory at a given path.
  /// @param path Path to a directory.
  void
  FileHandler::CreateDirectoryAt(std::filesystem::path const& path) const
  {
    mLogger->LogDebug(Log("CreateDirectoryAt", "FileHandler"));
    std::filesystem::create_directories(path);
  }

  /// @brief Opens a path into a file browser.
  /// @param path Path to open.
  void
  FileHandler::OpenPathInFileBrowser(std::filesystem::path const& path) const
  {
    mLogger->LogDebug(Log("OpenPathInFileBrowser", "FileHandler"));
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
  FileHandler::LaunchFile(std::filesystem::path path) const
  {
    mLogger->LogDebug(Log("LaunchFile", "FileHandler"));
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
                    std::filesystem::path const& to) const
  {
    mLogger->LogDebug(Log("Copy", "FileHandler"));
    std::filesystem::copy(from, to);
  }

  void
  FileHandler::Rename(std::filesystem::path const& oldPath,
                      std::filesystem::path const& newPath) const
  {
    mLogger->LogDebug(Log("Rename", "FileHandler"));
    std::filesystem::rename(oldPath, newPath);
  }

  void
  FileHandler::Duplicate(std::filesystem::path const& path) const
  {
    mLogger->LogDebug(Log("Duplicate", "FileHandler"));
    std::filesystem::path from = path;
    std::filesystem::path to = path;
    to = to.replace_filename(
      to.replace_extension("").filename().concat(" copy").concat(
        from.extension().string()));

    std::filesystem::copy(from, to);
  }

  void
  FileHandler::Delete(std::filesystem::path const& path) const
  {
    mLogger->LogDebug(Log("Delete", "FileHandler"));
    std::filesystem::remove(path);
  }

  auto
  FileHandler::ReadBinaryFileUnbuffered(std::filesystem::path const& path) const
    -> std::vector<unsigned char>
  {
#ifdef _WIN32
    HANDLE hFile =
      CreateFileA(path.string().c_str(),
                  GENERIC_READ,
                  FILE_SHARE_READ,
                  NULL,
                  OPEN_EXISTING,
                  FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN,
                  NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
      mLogger->LogError(Log(fmt::format("Error opening file: {} ({})",
                                        path.string(),
                                        GetLastError()),
                            "FileHandler"));
      return {};
    }

    // Get file size
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize))
    {
      mLogger->LogError(
        Log(fmt::format("Error getting file size: {}", GetLastError()),
            "FileHandler"));
      CloseHandle(hFile);
      return {};
    }

    constexpr size_t sectorSize = 4096; // Adjust based on system
    size_t           alignedSize =
      (fileSize.QuadPart + sectorSize - 1) & ~(sectorSize - 1);

    // Allocate sector-aligned memory
    void* rawBuffer = _aligned_malloc(alignedSize, sectorSize);
    if (rawBuffer == nullptr)
    {
      mLogger->LogError(
        Log(fmt::format("Memory allocation failed while reading file"),
            "FileHandler"));
      CloseHandle(hFile);
      return {};
    }

    DWORD bytesRead = 0;
    if (!::ReadFile(
          hFile, rawBuffer, static_cast<DWORD>(alignedSize), &bytesRead, NULL))
    {
      mLogger->LogError(Log(
        fmt::format("Error reading file: {}", GetLastError()), "FileHandler"));
      _aligned_free(rawBuffer);
      CloseHandle(hFile);
      return {};
    }

    CloseHandle(hFile);

    // Copy only the actual file size
    std::vector<unsigned char> buffer(static_cast<unsigned char*>(rawBuffer),
                                      static_cast<unsigned char*>(rawBuffer) +
                                        fileSize.QuadPart);
    _aligned_free(rawBuffer);

    return buffer;
#elif __linux__
    int fd = open(path.string().c_str(), O_RDONLY);
    if (fd == -1)
    {
      perror("open failed");
      return {};
    }

    // Get file size
    off_t fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    std::vector<unsigned char> buffer(fileSize);
    ssize_t                    bytesRead = read(fd, buffer.data(), fileSize);
    close(fd);

    if (bytesRead != fileSize)
    {
      mLogger->LogError(Log(
        fmt::format("Error reading file: {}", path.string()), "FileHandler"));
      return {};
    }

    return buffer;
#endif
  }
}