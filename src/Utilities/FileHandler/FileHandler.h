#pragma once
#include "pch.h"
#include "Logging/IDwarfLogger.h"
#include "IFileHandler.h"

namespace Dwarf
{
  /// @brief This class handles all file related tasks of the editor and project
  /// launcher.
  class FileHandler : public IFileHandler
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    /// @brief Path to the documents directory.
    std::filesystem::path m_DocumentsPath;

    /// @brief Path to the project settings directory.
    std::filesystem::path m_EngineSettingsPath;

    /// @brief Creates the platform depending path to the documents directory.
    /// @return Absolute path to the documents directory.
    std::filesystem::path
    CreateDocumentsFolderPath();

    /// @brief Creates the path to the project settings.
    /// @return Absolute path to where the project settings file is located.
    std::filesystem::path
    CreateEngineSettingsPath();

  public:
    FileHandler(std::shared_ptr<IDwarfLogger> logger);
    ~FileHandler() = default;
    /// @brief Returns the path to the document directory.
    /// @return An absolute path.
    std::filesystem::path
    GetDocumentsPath() override;

    /// @brief Returns the path to the project settings file.
    /// @return An absolute path.
    std::filesystem::path
    GetEngineSettingsPath() override;

    /// @brief Checks if a file is present at a given path.
    /// @param filePath Absolute path to a file.
    /// @return True if file exists, false if not.
    bool
    FileExists(std::filesystem::path const& filePath) override;

    /// @brief Reads a file and returns the content.
    /// @param filePath Absolute path to a file.
    /// @return The content of the file.
    std::string
    ReadFile(std::filesystem::path const& filePath) override;

    /// @brief Writes a string to a file at a given path.
    /// @param filePath Path where to write the string to.
    /// @param content Content to write.
    void
    WriteToFile(std::filesystem::path const& filePath,
                std::string_view             content) override;

    /// @brief Checks if a directory is present on the users disk.
    /// @param path Path to a directory.
    /// @return True if directory is present, false if not.
    bool
    DirectoryExists(std::filesystem::path const& path) override;

    /// @brief Creates a directory at a given path.
    /// @param path Path to a directory.
    void
    CreateDirectoryAt(std::filesystem::path const& path) override;

    /// @brief Opens a path into a file browser.
    /// @param path Path to open.
    void
    OpenPathInFileBrowser(std::filesystem::path const& path) override;

    void
    LaunchFile(std::filesystem::path path) override;

    void
    Copy(std::filesystem::path const& from,
         std::filesystem::path const& to) override;

    void
    Rename(std::filesystem::path const& oldPath,
           std::filesystem::path const& newPath) override;

    void
    Duplicate(std::filesystem::path const& path) override;

    void
    Delete(std::filesystem::path const& path) override;

    std::vector<unsigned char>
    ReadBinaryFileUnbuffered(std::filesystem::path const& path) override;
  };
}