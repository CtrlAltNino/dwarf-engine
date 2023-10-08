#pragma once

#include <nlohmann/json.hpp>
#include <filesystem>

#include "Utilities/FileHandler.h"

namespace Dwarf {


    /// @brief Utilities for Reading and writing meta data.
    class AssetMetaData {
        public:
            static constexpr const char* META_DATA_EXTENSION = ".dmeta";
            /// @brief Retrieves the meta data from an asset path.
            /// @param path Path to an asset.
            /// @return Metadata in JSON.
            static nlohmann::json GetMetaData(std::filesystem::path path){
                std::string fileContent = FileHandler::ReadFile(path.concat(META_DATA_EXTENSION));
                nlohmann::json jsonObject;

                if (!fileContent.empty()) {
                    jsonObject = nlohmann::json::parse(fileContent);
                }

                return jsonObject;
            }

            /// @brief Writes the given metadata to a path.
            /// @param path Path to write the metadata to.
            /// @param metaData The metadata in JSON.
            static void SetMetaData(std::filesystem::path path, nlohmann::json metaData){
                std::string fileContent = metaData.dump(4);

                if (!FileHandler::CheckIfDirectoyExists(path)) {
                    FileHandler::CreateDirectory(path);
                }

                FileHandler::WriteToFile(path.concat(META_DATA_EXTENSION), fileContent);
            }
    };
}