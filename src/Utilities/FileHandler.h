#pragma once

#include "Core/Base.h"

namespace Dwarf
{

  /// @brief This class handles all file related tasks of the editor and project
  /// launcher.
  class FileHandler
  {
  private:
    /// @brief Path to the documents directory.
    static std::filesystem::path s_DocumentsPath;

    /// @brief Path to the project settings.
    static std::filesystem::path s_ProjectSettingsPath;

    /// @brief Creates the platform depending path to the documents directory.
    /// @return Absolute path to the documents directory.
    static std::filesystem::path
    CreateDocumentsFolderPath();

    /// @brief Creates the path to the project settings.
    /// @return Absolute path to where the project settings file is located.
    static std::filesystem::path
    CreateProjectSettingsPath();

  public:
    /// @brief Returns the path to the document directory.
    /// @return An absolute path.
    static std::filesystem::path
    GetDocumentsPath();

    /// @brief Returns the path to the project settings file.
    /// @return An absolute path.
    static std::filesystem::path
    GetProjectSettingsPath();

    /// @brief Checks if a file is present at a given path.
    /// @param filePath Absolute path to a file.
    /// @return True if file exists, false if not.
    static bool
    CheckIfFileExists(std::filesystem::path const& filePath);

    /// @brief Reads a file and returns the content.
    /// @param filePath Absolute path to a file.
    /// @return The content of the file.
    static std::string
    ReadFile(std::filesystem::path const& filePath);

    /// @brief Writes a string to a file at a given path.
    /// @param filePath Path where to write the string to.
    /// @param content Content to write.
    static void
    WriteToFile(std::filesystem::path const& filePath,
                std::string_view             content);

    /// @brief Checks if a directory is present on the users disk.
    /// @param path Path to a directory.
    /// @return True if directory is present, false if not.
    static bool
    CheckIfDirectoyExists(std::filesystem::path const& path);

    /// @brief Creates a directory at a given path.
    /// @param path Path to a directory.
    static void
    CreateDirectory(std::filesystem::path const& path);

    /// @brief Opens a path into a file browser.
    /// @param path Path to open.
    static void
    OpenPathInFileBrowser(std::filesystem::path const& path);

    static void
    LaunchFile(std::filesystem::path path);

    static void
    Copy(std::filesystem::path const& from, std::filesystem::path const& to);

    static void
    Rename(std::filesystem::path const& oldPath,
           std::filesystem::path const& newPath);

    static void
    Duplicate(std::filesystem::path const& path);

    static void
    Delete(std::filesystem::path const& path);
  };
}