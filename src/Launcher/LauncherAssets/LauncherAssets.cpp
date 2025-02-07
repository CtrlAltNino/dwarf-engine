#include "LauncherAssets.h"
#include "Window/IWindow.h"

namespace Dwarf
{

#define GITHUB_PNG_ICON_PATH "data/engine/img/icons/github.png"
#define PATREON_PNG_ICON_PATH "data/engine/img/icons/patreon.png"
#define X_PNG_ICON_PATH "data/engine/img/icons/x.png"
#define APPLICATION_PNG_ICON_PATH "data/engine/img/icons/icon.png"

#define FONT_ROBOTO_REGULAR_PATH "data/engine/fonts/Roboto-Regular.ttf"
#define FONT_ROBOTO_LIGHT_PATH "data/engine/fonts/Roboto-Light.ttf"

  LauncherAssets::LauncherAssets(
    std::shared_ptr<IDwarfLogger>    logger,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IWindow>         window)
    : m_Logger(logger)
    , m_TextureFactory(textureFactory)
    , m_Window(window)
  {
    m_Logger->LogDebug(
      Log("Initializing project launcher assets", "LauncherAssets"));

    ImGuiIO io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    auto no_op_deleter = [](ImFont*) {};
    m_HeaderFont = std::shared_ptr<ImFont>(
      io.Fonts->AddFontFromFileTTF(FONT_ROBOTO_LIGHT_PATH, 26), no_op_deleter);
    m_TextFont = std::shared_ptr<ImFont>(
      io.Fonts->AddFontFromFileTTF(FONT_ROBOTO_REGULAR_PATH, 15),
      no_op_deleter);
    m_Logger->LogDebug(Log("Fonts loaded", "LauncherAssets"));

    m_GithubIcon =
      m_TextureFactory->FromPath(std::filesystem::path(GITHUB_PNG_ICON_PATH));
    m_PatreonIcon =
      m_TextureFactory->FromPath(std::filesystem::path(PATREON_PNG_ICON_PATH));
    m_XIcon =
      m_TextureFactory->FromPath(std::filesystem::path(X_PNG_ICON_PATH));
    m_Logger->LogDebug(Log("Icons loaded", "LauncherAssets"));

    m_Logger->LogDebug(
      Log("Project launcher assets initialized", "LauncherAssets"));
  }
  /// @brief Font loaded into IMGUI for header text
  std::shared_ptr<ImFont>
  LauncherAssets::GetHeaderFont()
  {
    return m_HeaderFont;
  }

  /// @brief Font loaded into IMGUI for regular text
  std::shared_ptr<ImFont>
  LauncherAssets::GetTextFont()
  {
    return m_TextFont;
  }

  /// @brief Loaded image for the github icon
  std::shared_ptr<ITexture>
  LauncherAssets::GetGithubIcon()
  {
    return m_GithubIcon;
  }

  /// @brief Loaded image for the patreon icon
  std::shared_ptr<ITexture>
  LauncherAssets::GetPatreonIcon()
  {
    return m_PatreonIcon;
  }

  /// @brief Loaded image for the X/Twitter icon
  std::shared_ptr<ITexture>
  LauncherAssets::GetXIcon()
  {
    return m_XIcon;
  }
}