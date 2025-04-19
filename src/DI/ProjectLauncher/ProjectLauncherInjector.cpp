#include "pch.hpp"

#include "Core/Asset/Metadata/AssetMetadata.hpp"
#include "Core/Asset/Texture/IImageFileLoader.hpp"
#include "Core/Asset/Texture/ImageFileLoader.hpp"
#include "Core/Asset/Texture/TextureWorker/TextureLoadingWorker.hpp"
#include "Core/Base.hpp"
#include "Core/Rendering/GraphicsContext/GraphicsContextFactory.hpp"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "Core/Rendering/Texture/TextureFactory.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
#include "Core/Rendering/VramTracker/VramTracker.hpp"
#include "DI/ProjectLauncher/ProjectLauncherInjector.hpp"
#include "Editor/Stats/EditorStats.hpp"
#include "Input/IInputManager.hpp"
#include "Input/InputManager.hpp"
#include "Launcher/LauncherAssets/ILauncherAssets.hpp"
#include "Launcher/LauncherAssets/LauncherAssets.hpp"
#include "Launcher/LauncherData/LauncherData.hpp"
#include "Launcher/ProjectCreator/IProjectCreator.hpp"
#include "Launcher/ProjectCreator/ProjectCreator.hpp"
#include "Launcher/ProjectLauncher.hpp"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.hpp"
#include "Launcher/SavedProjects/IO/SavedProjectsIO.hpp"
#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include "Launcher/SavedProjects/SavedProjects.hpp"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.hpp"
#include "Launcher/SavedProjects/Sorter/SavedProjectsSorter.hpp"
#include "Launcher/View/ButtonsView/ButtonsView.hpp"
#include "Launcher/View/ButtonsView/IButtonsView.hpp"
#include "Launcher/View/ChangeGraphicsApiModal/ChangeGraphicsApiModal.hpp"
#include "Launcher/View/ChangeGraphicsApiModal/IChangeGraphicsApiModal.hpp"
#include "Launcher/View/CreateNewProjectModal/CreateNewProjectModal.hpp"
#include "Launcher/View/CreateNewProjectModal/ICreateNewProjectModal.hpp"
#include "Launcher/View/FooterView/FooterView.hpp"
#include "Launcher/View/FooterView/IFooterView.hpp"
#include "Launcher/View/IProjectLauncherView.hpp"
#include "Launcher/View/ProjectLauncherView.hpp"
#include "Launcher/View/ProjectListView/IProjectListView.hpp"
#include "Launcher/View/ProjectListView/ProjectListView.hpp"
#include "Launcher/View/ProjectNotFoundModal/IProjectNotFoundModal.hpp"
#include "Launcher/View/ProjectNotFoundModal/ProjectNotFoundModal.hpp"
#include "Logging/DwarfLogger.hpp"
#include "Project/ProjectSettingsIO.hpp"
#include "UI/IImGuiLayerFactory.hpp"
#include "UI/ImGuiLayerFactory.hpp"
#include "Utilities/FileHandler/FileHandler.hpp"
#include "Window/IWindow.hpp"
#include "Window/SDL3Window.hpp"

namespace Dwarf
{
  auto
  ProjectLauncherInjector::CreateInjector()
    -> boost::di::injector<std::shared_ptr<ProjectLauncher>>
  {
    return boost::di::make_injector<boost::di::extension::shared_config>(
      boost::di::bind<LogName>.to(LogName("Project launcher")),
      boost::di::bind<IDwarfLogger>.to<DwarfLogger>().in(
        boost::di::extension::shared),
      boost::di::bind<GraphicsApi>.to(GraphicsApi::OpenGL),
      boost::di::bind<IFileHandler>.to<FileHandler>().in(
        boost::di::extension::shared),
      boost::di::bind<IAssetMetadata>.to<AssetMetadata>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectSettingsIO>.to<ProjectSettingsIO>().in(
        boost::di::extension::shared),
      boost::di::bind<ImGuiIniFilePath>.to(ImGuiIniFilePath("./data")),
      boost::di::bind<IImGuiLayerFactory>.to<ImGuiLayerFactory>().in(
        boost::di::extension::shared),
      boost::di::bind<IVramTracker>.to<VramTracker>().in(
        boost::di::extension::shared),
      boost::di::bind<IEditorStats>.to<EditorStats>().in(
        boost::di::extension::shared),
      boost::di::bind<WindowProps>.to(
        WindowProps("Dwarf Engine - Project Launcher",
                    1100,
                    600,
                    GraphicsApi::OpenGL,
                    false)),
      boost::di::bind<ILauncherData>.to<LauncherData>().in(
        boost::di::extension::shared),
      boost::di::bind<IWindow>.to<SDL3Window>().in(
        boost::di::extension::shared),
      boost::di::bind<IGraphicsContextFactory>.to<GraphicsContextFactory>().in(
        boost::di::extension::shared),
      boost::di::bind<IInputManager>.to<InputManager>().in(
        boost::di::extension::shared),
      boost::di::bind<IButtonsView>.to<ButtonsView>().in(
        boost::di::extension::shared),
      boost::di::bind<IChangeGraphicsApiModal>.to<ChangeGraphicsApiModal>().in(
        boost::di::extension::shared),
      boost::di::bind<ICreateNewProjectModal>.to<CreateNewProjectModal>().in(
        boost::di::extension::shared),
      boost::di::bind<IFooterView>.to<FooterView>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectListView>.to<ProjectListView>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectNotFoundModal>.to<ProjectNotFoundModal>().in(
        boost::di::extension::shared),
      boost::di::bind<ILauncherAssets>.to<LauncherAssets>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectLauncherView>.to<ProjectLauncherView>().in(
        boost::di::unique),
      boost::di::bind<ITextureFactory>.to<TextureFactory>().in(
        boost::di::extension::shared),
      boost::di::bind<IImageFileLoader>.to<ImageFileLoader>().in(
        boost::di::extension::shared),
      boost::di::bind<ITextureLoadingWorker>.to<TextureLoadingWorker>().in(
        boost::di::extension::shared),
      boost::di::bind<ISavedProjects>.to<SavedProjects>().in(
        boost::di::extension::shared),
      boost::di::bind<ISavedProjectsIO>.to<SavedProjectsIO>().in(
        boost::di::extension::shared),
      boost::di::bind<ISavedProjectsSorter>.to<SavedProjectsSorter>().in(
        boost::di::extension::shared),
      boost::di::bind<IProjectCreator>.to<ProjectCreator>().in(
        boost::di::extension::shared));
  }
}