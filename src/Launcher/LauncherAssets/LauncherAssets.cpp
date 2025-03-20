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
    : mLogger(logger)
    , mTextureFactory(textureFactory)
    , mWindow(window)
  {
    mLogger->LogDebug(
      Log("Initializing project launcher assets", "LauncherAssets"));

    ImGuiIO io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    auto no_op_deleter = [](ImFont*) {};
    mHeaderFont = std::shared_ptr<ImFont>(
      io.Fonts->AddFontFromFileTTF(FONT_ROBOTO_LIGHT_PATH, 26), no_op_deleter);
    mTextFont = std::shared_ptr<ImFont>(
      io.Fonts->AddFontFromFileTTF(FONT_ROBOTO_REGULAR_PATH, 15),
      no_op_deleter);
    mLogger->LogDebug(Log("Fonts loaded", "LauncherAssets"));

    mGithubIcon =
      mTextureFactory->FromPath(std::filesystem::path(GITHUB_PNG_ICON_PATH));
    mPatreonIcon =
      mTextureFactory->FromPath(std::filesystem::path(PATREON_PNG_ICON_PATH));
    mXIcon = mTextureFactory->FromPath(std::filesystem::path(X_PNG_ICON_PATH));
    mLogger->LogDebug(Log("Icons loaded", "LauncherAssets"));

    mLogger->LogDebug(
      Log("Project launcher assets initialized", "LauncherAssets"));
  }
  /// @brief Font loaded into IMGUI for header text
  std::shared_ptr<ImFont>
  LauncherAssets::GetHeaderFont()
  {
    return mHeaderFont;
  }

  /// @brief Font loaded into IMGUI for regular text
  std::shared_ptr<ImFont>
  LauncherAssets::GetTextFont()
  {
    return mTextFont;
  }

  /// @brief Loaded image for the github icon
  std::shared_ptr<ITexture>
  LauncherAssets::GetGithubIcon()
  {
    return mGithubIcon;
  }

  /// @brief Loaded image for the patreon icon
  std::shared_ptr<ITexture>
  LauncherAssets::GetPatreonIcon()
  {
    return mPatreonIcon;
  }

  /// @brief Loaded image for the X/Twitter icon
  std::shared_ptr<ITexture>
  LauncherAssets::GetXIcon()
  {
    return mXIcon;
  }
}