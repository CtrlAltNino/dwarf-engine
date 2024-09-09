#include "DI/ProjectLauncher/ProjectLauncherInjector.h"
#include "Logging/DwarfLogger.h"
#include "Launcher/ProjectLauncherData.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Asset/Texture/ImageFileLoader.h"
#include "UI/IImGuiLayerFactory.h"
#include "UI/ImGuiLayerFactory.h"
#include "Core/Base.h"
#include "Core/Rendering/GraphicsContext/GraphicsContextFactory.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Rendering/Texture/TextureFactory.h"
#include "Input/IInputManager.h"
#include "Input/InputManager.h"
#include "Launcher/ProjectLauncher.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Launcher/View/ProjectLauncherView.h"
#include "Window/IWindow.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/ProjectList.h"
#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Launcher/ProjectList/IO/ProjectListIO.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"
#include "Launcher/ProjectList/Sorter/ProjectListSorter.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/ProjectCreator/ProjectCreator.h"
#include <boost/di.hpp>
#include <boost/di/extension/scopes/shared.hpp>

#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#elif __linux__
#include "Platform/Linux/LinuxWindow.h"
#endif

namespace Dwarf
{
  boost::di::injector<std::shared_ptr<ProjectLauncher>>
  ProjectLauncherInjector::CreateInjector()
  {
    return boost::di::make_injector<boost::di::extension::shared_config>(
      boost::di::bind<LogName>.to(LogName("Project launcher")),
      boost::di::bind<IDwarfLogger>.to<DwarfLogger>().in(
        boost::di::extension::shared),
      boost::di::bind<GraphicsApi>.to(GraphicsApi::OpenGL),
      boost::di::bind<IImGuiLayerFactory>.to<ImGuiLayerFactory>().in(
        boost::di::extension::shared),
      boost::di::bind<WindowProps>.to(WindowProps(
        "Dwarf Engine - Project Launcher", 1100, 600, GraphicsApi::OpenGL)),
      boost::di::bind<IProjectLauncherData>.to<ProjectLauncherData>().in(
        boost::di::extension::shared),
#ifdef _WIN32
      boost::di::bind<IWindow>.to<WindowsWindow>().in(
        boost::di::extension::shared),
#elif __linux__
      boost::di::bind<IWindow>.to<LinuxWindow>().in(boost::di::singleton),
#endif
      boost::di::bind<IGraphicsContextFactory>.to<GraphicsContextFactory>().in(
        boost::di::extension::shared),
      boost::di::bind<IInputManager>.to<InputManager>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectLauncherView>.to<ProjectLauncherView>().in(
        boost::di::unique),
      boost::di::bind<ITextureFactory>.to<TextureFactory>().in(
        boost::di::extension::shared),
      boost::di::bind<IImageFileLoader>.to<ImageFileLoader>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectList>.to<ProjectList>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectListIO>.to<ProjectListIO>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectListSorter>.to<ProjectListSorter>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectCreator>.to<ProjectCreator>().in(
        boost::di::extension::shared));
  }
}