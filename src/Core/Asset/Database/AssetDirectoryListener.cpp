#include "AssetDirectoryListener.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Rendering/Shader/IShader.h"

namespace Dwarf
{
  AssetDirectoryListener::AssetDirectoryListener(
    AssetDirectoryPath const& assetDirectoryPath)
    : m_FileWatcher()
  {
    m_WatchID =
      m_FileWatcher.addWatch(assetDirectoryPath.t.string(), this, true);
    // m_WatchID = m_FileWatcher.addWatch(
    //   Shader::GetDefaultShaderPath().string(), this, true);

    // Start watching asynchronously the directories
    m_FileWatcher.watch();
  }
  AssetDirectoryListener::~AssetDirectoryListener() {}

  void
  AssetDirectoryListener::handleFileAction(efsw::WatchID      watchid,
                                           const std::string& dir,
                                           const std::string& filename,
                                           efsw::Action       action,
                                           std::string        oldFilename)
  {

    switch (action)
    {
      case efsw::Actions::Add:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Added" << std::endl;

        for (const auto& callback : m_AddFileCallbacks)
        {
          callback(dir, filename);
        }
        break;
      case efsw::Actions::Delete:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Delete" << std::endl;
        for (const auto& callback : m_DeleteFileCallbacks)
        {
          callback(dir, filename);
        }
        break;
      case efsw::Actions::Modified:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Modified" << std::endl;
        for (const auto& callback : m_ModifyFileCallbacks)
        {
          callback(dir, filename);
        }
        break;
      case efsw::Actions::Moved:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Moved from (" << oldFilename << ")"
                  << std::endl;
        for (const auto& callback : m_MoveFileCallbacks)
        {
          callback(dir, filename, oldFilename);
        }
        break;
      default: std::cout << "Should never happen!" << std::endl;
    }
  }
}