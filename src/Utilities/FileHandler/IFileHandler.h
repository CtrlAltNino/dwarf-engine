#pragma once
#include "pch.h"

namespace Dwarf
{
  /// @brief This class handles all file related tasks of the editor and project
  /// launcher.
  class IFileHandler
  {
  public:
    virtual ~IFileHandler() = default;
    /// @brief Returns the path to the document directory.
    /// @return An absolute path.
    [[nodiscard]] virtual auto
    GetDocumentsPath() const -> std::filesystem::path = 0;

    /// @brief Returns the path to the engine settings directory.
    /// @return An absolute path.
    [[nodiscard]] virtual auto
    GetEngineSettingsPath() const -> std::filesystem::path = 0;

    /// @brief Checks if a file is present at a given path.
    /// @param filePath Absolute path to a file.
    /// @return True if file exists, false if not.
    [[nodiscard]] virtual auto
    FileExists(std::filesystem::path const& filePath) const -> bool = 0;

    /// @brief Reads a file and returns the content.
    /// @param filePath Absolute path to a file.
    /// @return The content of the file.
    [[nodiscard]] virtual auto
    ReadFile(std::filesystem::path const& filePath) const -> std::string = 0;

    /// @brief Writes a string to a file at a given path.
    /// @param filePath Path where to write the string to.
    /// @param content Content to write.
    virtual void
    WriteToFile(std::filesystem::path const& filePath,
                std::string_view             content) const = 0;

    /// @brief Checks if a directory is present on the users disk.
    /// @param path Path to a directory.
    /// @return True if directory is present, false if not.
    virtual auto
    DirectoryExists(std::filesystem::path const& path) const -> bool = 0;

    /// @brief Creates a directory at a given path.
    /// @param path Path to a directory.
    virtual void
    CreateDirectoryAt(std::filesystem::path const& path) const = 0;

    /// @brief Opens a path into a file browser.
    /// @param path Path to open.
    virtual void
    OpenPathInFileBrowser(std::filesystem::path const& path) const = 0;

    virtual void
    LaunchFile(std::filesystem::path path) const = 0;

    virtual void
    Copy(std::filesystem::path const& from,
         std::filesystem::path const& to) const = 0;

    virtual void
    Rename(std::filesystem::path const& oldPath,
           std::filesystem::path const& newPath) const = 0;

    virtual void
    Duplicate(std::filesystem::path const& path) const = 0;

    virtual void
    Delete(std::filesystem::path const& path) const = 0;

    [[nodiscard]] virtual auto
    ReadBinaryFileUnbuffered(std::filesystem::path const& path) const
      -> std::vector<unsigned char> = 0;
  };
}