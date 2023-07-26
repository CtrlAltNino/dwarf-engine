#pragma once

#ifdef _WIN32
    #include <Windows.h>
    #include <KnownFolders.h>
    #include <ShlObj.h>
    #include <filesystem>
#endif

namespace Dwarf {

    /// @brief Utility class to open links in the browser for different platforms/OS.
    class BrowserLinkOpener {
        public:
            /// @brief Opens a link in the browser of the user.
            /// @param link
            static void OpenLink(const char* link){
                #if _WIN32
                    const size_t cSize = strlen(link)+1;
                    wchar_t* wc = new wchar_t[cSize];
                    mbstowcs (wc, link, cSize);
                    ShellExecute(0, 0, wc, 0, 0, SW_SHOW);
                #endif
            }
    };
}