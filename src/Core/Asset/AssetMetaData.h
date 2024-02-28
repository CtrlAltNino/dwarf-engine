#pragma once

#include <nlohmann/json.hpp>
#include <filesystem>

#include "Utilities/FileHandler.h"

namespace Dwarf
{

    /// @brief Utilities for Reading and writing meta data.
    class AssetMetaData
    {
    public:
        static constexpr const char *META_DATA_EXTENSION = ".dmeta";

    private:
        static std::filesystem::path GetMetaDataPath(std::filesystem::path assetPath)
        {
            return assetPath.concat(META_DATA_EXTENSION);
        }

    public:
        /// @brief Retrieves the meta data from an asset path.
        /// @param assetPath Path to an asset.
        /// @return Metadata in JSON.
        static nlohmann::json GetMetaData(const std::filesystem::path &assetPath)
        {
            std::string fileContent = FileHandler::ReadFile(GetMetaDataPath(assetPath));
            nlohmann::json jsonObject;

            if (!fileContent.empty())
            {
                jsonObject = nlohmann::json::parse(fileContent);
            }

            return jsonObject;
        }

        /// @brief Writes the given metadata to a path.
        /// @param assetPath Path to write the metadata to.
        /// @param metaData The metadata in JSON.
        static void SetMetaData(const std::filesystem::path &assetPath, nlohmann::json metaData)
        {
            std::string fileContent = metaData.dump(4);

            if (!FileHandler::CheckIfDirectoyExists(assetPath))
            {
                FileHandler::CreateDirectory(assetPath);
            }

            FileHandler::WriteToFile(GetMetaDataPath(assetPath), fileContent);
        }

        /// @brief Removes the metadata file of an asset.
        /// @param assetPath Path to an asset.
        static void RemoveMetaData(const std::filesystem::path &assetPath)
        {
            FileHandler::Delete(GetMetaDataPath(assetPath));
        }

        static void Rename(const std::filesystem::path &from, const std::filesystem::path &to)
        {
            FileHandler::Rename(GetMetaDataPath(from), GetMetaDataPath(to));
        }
    };
}