#pragma once
#include "pch.h"
#include <fmt/format.h>

#ifdef _WIN32
#include <Windows.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#endif

#ifdef __linux__
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace Dwarf
{

  /// @brief Utility class to open links in the browser for different
  /// platforms/OS.
  class BrowserLinkOpener
  {
  public:
    /// @brief Opens a link in the browser of the user.
    /// @param link
    static void
    OpenLink(std::string link)
    {
#if _WIN32
      const size_t cSize = strlen(link) + 1;
      wchar_t*     wc = new wchar_t[cSize];
      size_t       convertedChars = 0;
      mbstowcs_s(&convertedChars, wc, cSize, link, cSize);

      ShellExecute(0, 0, wc, 0, 0, SW_SHOW);
      delete[] (wc);
#endif
#if __linux__
      pid_t pid = fork();

      if (pid == 0)
      { // Child process
        execlp("xdg-open", "xdg-open", link.c_str(), nullptr);
      }
      else if (pid > 0)
      {                           // Parent process
        waitpid(pid, nullptr, 0); // Wait for the child process to complete
      }
      else
      {
        // Fork failed
        perror("fork failed");
      }
#endif
    }
  };
}