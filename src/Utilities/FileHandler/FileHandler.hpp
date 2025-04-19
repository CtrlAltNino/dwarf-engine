#pragma once

#include "IFileHandler.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  /// @brief This class handles all file related tasks of the editor and project
  /// launcher.
  class FileHandler : public IFileHandler
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    /// @brief Path to the documents directory.
    std::filesystem::path mDocumentsPath;

    /// @brief Path to the project settings directory.
    std::filesystem::path mEngineSettingsPath;

    /// @brief Creates the platform depending path to the documents directory.
    /// @return Absolute path to the documents directory.
    auto
    CreateDocumentsFolderPath() const -> std::filesystem::path;

    /// @brief Creates the path to the project settings.
    /// @return Absolute path to where the project settings file is located.
    auto
    CreateEngineSettingsPath() const -> std::filesystem::path;

  public:
    FileHandler(std::shared_ptr<IDwarfLogger> logger);
    ~FileHandler() = default;
    /// @brief Returns the path to the document directory.
    /// @return An absolute path.
    [[nodiscard]] auto
    GetDocumentsPath() const -> std::filesystem::path override;

    /// @brief Returns the path to the project settings file.
    /// @return An absolute path.
    [[nodiscard]] auto
    GetEngineSettingsPath() const -> std::filesystem::path override;

    /// @brief Checks if a file is present at a given path.
    /// @param filePath Absolute path to a file.
    /// @return True if file exists, false if not.
    [[nodiscard]] auto
    FileExists(std::filesystem::path const& filePath) const -> bool override;

    /// @brief Reads a file and returns the content.
    /// @param filePath Absolute path to a file.
    /// @return The content of the file.
    [[nodiscard]] auto
    ReadFile(std::filesystem::path const& filePath) const
      -> std::string override;

    /// @brief Writes a string to a file at a given path.
    /// @param filePath Path where to write the string to.
    /// @param content Content to write.
    void
    WriteToFile(std::filesystem::path const& filePath,
                std::string_view             content) const override;

    /// @brief Checks if a directory is present on the users disk.
    /// @param path Path to a directory.
    /// @return True if directory is present, false if not.
    [[nodiscard]] auto
    DirectoryExists(std::filesystem::path const& path) const -> bool override;

    /// @brief Creates a directory at a given path.
    /// @param path Path to a directory.
    void
    CreateDirectoryAt(std::filesystem::path const& path) const override;

    /// @brief Opens a path into a file browser.
    /// @param path Path to open.
    void
    OpenPathInFileBrowser(std::filesystem::path const& path) const override;

    void
    LaunchFile(std::filesystem::path path) const override;

    void
    Copy(std::filesystem::path const& from,
         std::filesystem::path const& to) const override;

    void
    Rename(std::filesystem::path const& oldPath,
           std::filesystem::path const& newPath) const override;

    void
    Duplicate(std::filesystem::path const& path) const override;

    void
    Delete(std::filesystem::path const& path) const override;

    [[nodiscard]] auto
    ReadBinaryFileUnbuffered(std::filesystem::path const& path) const
      -> std::vector<unsigned char> override;
  };
}