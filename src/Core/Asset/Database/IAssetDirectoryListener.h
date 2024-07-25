#pragma once

namespace Dwarf
{
  class IAssetDirectoryListener
  {
  public:
    virtual ~IAssetDirectoryListener() = default;

    virtual void
    registerAddFileCallback(
      std::function<void(const std::string& dir, const std::string& filename)>
        callback) = 0;
    virtual void
    registerDeleteFileCallback(
      std::function<void(const std::string& dir, const std::string& filename)>
        callback) = 0;
    virtual void
    registerModifyFileCallback(
      std::function<void(const std::string& dir, const std::string& filename)>
        callback) = 0;
    virtual void
    registerMoveFileCallback(
      std::function<void(const std::string& dir,
                         const std::string& filename,
                         std::string        oldFilename)> callback) = 0;
  };
}