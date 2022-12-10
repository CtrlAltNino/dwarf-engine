#pragma once
#include"../../utilities/dpch.h"
#include<efsw/efsw.hpp>

/// @brief EFSW listener class
class AssetDirectoryListener : public efsw::FileWatchListener {
    void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename ) override
	{
		switch( action )
		{
		case efsw::Actions::Add:
			std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added" << std::endl;
			break;
		case efsw::Actions::Delete:
			std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
			break;
		case efsw::Actions::Modified:
			std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified" << std::endl;
			break;
		case efsw::Actions::Moved:
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from (" << oldFilename << ")" << std::endl;
			break;
		default:
			std::cout << "Should never happen!" << std::endl;
		}
	}
};