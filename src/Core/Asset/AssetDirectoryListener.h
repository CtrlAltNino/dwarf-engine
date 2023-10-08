#pragma once

#include <efsw/efsw.hpp>
#include "Core/Asset/AssetDatabase.h"

namespace Dwarf
{

	/// @brief EFSW listener class
	class AssetDirectoryListener : public efsw::FileWatchListener
	{

		void handleFileAction(efsw::WatchID watchid, const std::string &dir, const std::string &filename, efsw::Action action, std::string oldFilename) override
		{
			std::filesystem::path path = std::filesystem::path(dir) / std::filesystem::path(filename);
			switch (action)
			{
			case efsw::Actions::Add:
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added" << std::endl;
				if (path.has_extension() && path.extension() != AssetMetaData::META_DATA_EXTENSION)
				{
					AssetDatabase::Import(path);
				}
				break;
			case efsw::Actions::Delete:
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
				break;
			case efsw::Actions::Modified:
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified" << std::endl;

				if (path.has_extension() && path.extension() != AssetMetaData::META_DATA_EXTENSION)
				{
					AssetDatabase::Reimport(path);
					switch (AssetDatabase::GetType(path))
					{
					case ASSET_TYPE::COMPUTE_SHADER:
					case ASSET_TYPE::FRAGMENT_SHADER:
					case ASSET_TYPE::GEOMETRY_SHADER:
					case ASSET_TYPE::HLSL_SHADER:
					case ASSET_TYPE::TESC_SHADER:
					case ASSET_TYPE::TESE_SHADER:
					case ASSET_TYPE::VERTEX_SHADER:
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
						Ref<AssetReference<MaterialAsset>> mat = AssetDatabase::Retrieve<MaterialAsset>(path);
						AssetDatabase::AddShaderToRecompilationQueue(mat->GetAsset()->m_Material->GetShader().get());
					}
					}
				}

				break;
			case efsw::Actions::Moved:
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from (" << oldFilename << ")" << std::endl;
				break;
			default:
				std::cout << "Should never happen!" << std::endl;
			}
		}
	};
}