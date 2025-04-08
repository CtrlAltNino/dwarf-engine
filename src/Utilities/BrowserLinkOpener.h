#pragma once

#ifdef _WIN32
#include <Windows.h>

#include <KnownFolders.h>
#include <ShlObj.h>

#endif

#ifdef __linux__
#include <sys/wait.h>
#include <unistd.h>

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
      const size_t cSize = strlen(link.c_str()) + 1;
      auto*        wideChar = new wchar_t[cSize];
      size_t       convertedChars = 0;
      mbstowcs_s(&convertedChars, wideChar, cSize, link.c_str(), cSize);

      ShellExecute(0, 0, wideChar, 0, 0, SW_SHOW);
      delete[] (wideChar);
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