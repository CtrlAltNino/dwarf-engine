#pragma once

#include "../../utilities/dpch.h"

class PathUtilities {
    public:
        static char* ConvertBackSlashesToForwardSlashes(std::string path){
            size_t pos;
            char *cstr = new char[path.length() + 1];
            
            while ((pos = path.find('\\')) != std::string::npos) {
                path.replace(pos, 1, "/");
            }
            
            strcpy(cstr, path.c_str());

            return cstr;
        }
};