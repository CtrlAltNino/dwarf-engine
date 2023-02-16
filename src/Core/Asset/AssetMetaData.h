#pragma once

#include <nlohmann/json.hpp>
#include <filesystem>

#include "Utilities/FileHandler.h"

namespace Dwarf {
    
    /// @brief Utilities for Reading and writing meta data.
    class AssetMetaData {
        public:
            /// @brief Retrieves the meta data from an asset path.
            /// @param path Path to an asset.
            /// @return Metadata in JSON.
            static nlohmann::json GetMetaData(std::filesystem::path path){
                std::string fileContent = FileHandler::readFile((path.string() + ".meta").c_str());
                nlohmann::json jsonObject;

                if (!fileContent.empty()) {
                    jsonObject = nlohmann::json::parse(fileContent);
                }

                return jsonObject;
            }
            
            /// @brief Writes the given metadata to a path.
            /// @param path Path to write the metadata to.
            /// @param metaData The metadata in JSON.
            static void SetMetaData(std::string path, nlohmann::json metaData){
                std::string fileContent = metaData.dump(4);
                
                if (!FileHandler::checkIfDirectoyExists(path)) {
                    FileHandler::createDirectoryS(path);
                }
                
                FileHandler::writeToFile((path + ".meta").c_str(), fileContent);
            }
    };
}