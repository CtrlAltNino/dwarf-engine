#pragma once

#include <string>

namespace Dwarf {

    /// @brief Utility functionalities for paths.
    class PathUtilities {
        public:
            /// @brief Converts the backslashes in a path to forward slashes.
            /// @param path Path to convert.
            /// @return Converted path.
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
}