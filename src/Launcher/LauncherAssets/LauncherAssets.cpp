#include "pch.h"

#include "LauncherAssets.h"
#include "Window/IWindow.h"

namespace Dwarf
{

#define GITHUB_PNG_ICON_PATH "data/engine/img/icons/github.png"
#define APPLICATION_PNG_ICON_PATH "data/engine/img/icons/icon.png"

#define FONT_ROBOTO_REGULAR_PATH "data/engine/fonts/Roboto-Regular.ttf"
#define FONT_ROBOTO_LIGHT_PATH "data/engine/fonts/Roboto-Light.ttf"

  LauncherAssets::LauncherAssets(
    std::shared_ptr<IDwarfLogger>    logger,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IWindow>         window)
    : mLogger(std::move(logger))
    , mTextureFactory(std::move(textureFactory))
    , mWindow(std::move(window))
  {
    mLogger->LogDebug(
      Log("Initializing project launcher assets", "LauncherAssets"));

    ImGuiIO io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    auto noOpDeleter = [](ImFont*) {};
    mHeaderFont = std::shared_ptr<ImFont>(
      io.Fonts->AddFontFromFileTTF(FONT_ROBOTO_LIGHT_PATH, 26), noOpDeleter);
    mTextFont = std::shared_ptr<ImFont>(
      io.Fonts->AddFontFromFileTTF(FONT_ROBOTO_REGULAR_PATH, 15), noOpDeleter);
    mLogger->LogDebug(Log("Fonts loaded", "LauncherAssets"));

    mGithubIcon =
      mTextureFactory->FromPath(std::filesystem::path(GITHUB_PNG_ICON_PATH));
    mLogger->LogDebug(Log("Icons loaded", "LauncherAssets"));

    mLogger->LogDebug(
      Log("Project launcher assets initialized", "LauncherAssets"));
  }

  /// @brief Font loaded into IMGUI for header text
  auto
  LauncherAssets::GetHeaderFont() -> std::shared_ptr<ImFont>
  {
    return mHeaderFont;
  }

  /// @brief Font loaded into IMGUI for regular text
  auto
  LauncherAssets::GetTextFont() -> std::shared_ptr<ImFont>
  {
    return mTextFont;
  }

  /// @brief Loaded image for the github icon
  auto
  LauncherAssets::GetGithubIcon() -> std::shared_ptr<ITexture>
  {
    return mGithubIcon;
  }
}