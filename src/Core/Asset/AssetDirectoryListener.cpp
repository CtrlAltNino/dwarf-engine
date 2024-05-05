#include "dpch.h"

#include "Core/Asset/AssetDirectoryListener.h"
#include "Core/Asset/AssetDatabase.h"

namespace Dwarf
{
  AssetDirectoryListener::AssetDirectoryListener() = default;
  AssetDirectoryListener::~AssetDirectoryListener() = default;

  void
  AssetDirectoryListener::handleFileAction(efsw::WatchID      watchid,
                                           const std::string& dir,
                                           const std::string& filename,
                                           efsw::Action       action,
                                           std::string        oldFilename)
  {
    std::filesystem::path path =
      std::filesystem::path(dir) / std::filesystem::path(filename);
    switch (action)
    {
      case efsw::Actions::Add:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Added" << std::endl;
        if (path.has_extension() &&
            path.extension() != AssetMetaData::META_DATA_EXTENSION)
        {
          AssetDatabase::Import(path);
        }
        break;
      case efsw::Actions::Delete:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Delete" << std::endl;
        break;
      case efsw::Actions::Modified:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Modified" << std::endl;

        if (path.has_extension() &&
            path.extension() != AssetMetaData::META_DATA_EXTENSION)
        {
          AssetDatabase::Reimport(path);
          switch (AssetDatabase::GetType(path))
          {
            using enum ASSET_TYPE;
            case COMPUTE_SHADER:
            case FRAGMENT_SHADER:
            case GEOMETRY_SHADER:
            case HLSL_SHADER:
            case TESC_SHADER:
            case TESE_SHADER:
            case VERTEX_SHADER:
              {
                if (AssetDatabase::s_ShaderAssetMap.contains(path))
                {
                  std::cout << "A shader asset has been updated!" << std::endl;
                  AssetDatabase::AddShaderToRecompilationQueue(path);
                }
                break;
              }
            case ASSET_TYPE::MATERIAL:
              {
                std::cout << "A material asset has been updated!" << std::endl;
                std::shared_ptr<AssetReference<MaterialAsset>> mat =
                  AssetDatabase::Retrieve<MaterialAsset>(path);
                AssetDatabase::AddShaderToRecompilationQueue(
                  mat->GetAsset()->m_Material->GetShader());
                break;
              }
            case ASSET_TYPE::MODEL:
              {
                // TODO: REIMPORT MODEL FILE
                std::cout << "A model asset has been updated!" << std::endl;
                break;
              }
            case ASSET_TYPE::TEXTURE:
              {
                // TODO: REIMPORT TEXTURE
                std::cout << "A texture asset has been updated!" << std::endl;
                break;
              }
            case ASSET_TYPE::SCENE:
              {
                // TODO: IF ITS THE CURRENTLY OPEN SCENE, MODAL TO ASK IF IT
                // SHOULD BE RELOADED
                std::cout << "A scene asset has been updated!" << std::endl;
                break;
              }
            case ASSET_TYPE::UNKNOWN:
              {
                std::cout << "An unsupported asset has been updated!"
                          << std::endl;
                break;
              }
          }
        }

        break;
      case efsw::Actions::Moved:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Moved from (" << oldFilename << ")"
                  << std::endl;
        break;
      default: std::cout << "Should never happen!" << std::endl;
    }
  }
}