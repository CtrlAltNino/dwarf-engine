#pragma once
#include <nlohmann/json.hpp>
#include "../../utilities/FileHandler.h"

class AssetMetaDataHelper {
    public:
        static nlohmann::json GetMetaData(std::filesystem::path path){
            std::string fileContent = FileHandler::readFile((path.string() + ".meta").c_str());
            nlohmann::json jsonObject;

            if (!fileContent.empty()) {
                jsonObject = nlohmann::json::parse(fileContent);
            }

            return jsonObject;
        }
        
        static void SetMetaData(std::string path, nlohmann::json metaData){
            std::string fileContent = metaData.dump(4);
            
            if (!FileHandler::checkIfDirectoyExists(path)) {
                FileHandler::createDirectoryS(path);
            }
            
            FileHandler::writeToFile((path + ".meta").c_str(), fileContent);
        }
};