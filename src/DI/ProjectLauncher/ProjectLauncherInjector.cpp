#include "pch.h"

#include "Core/Asset/Metadata/AssetMetadata.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Asset/Texture/ImageFileLoader.h"
#include "Core/Asset/Texture/TextureWorker/TextureLoadingWorker.h"
#include "Core/Base.h"
#include "Core/Rendering/GraphicsContext/GraphicsContextFactory.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Rendering/Texture/TextureFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Core/Rendering/VramTracker/VramTracker.h"
#include "DI/ProjectLauncher/ProjectLauncherInjector.h"
#include "Editor/Stats/EditorStats.h"
#include "Input/IInputManager.h"
#include "Input/InputManager.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/LauncherAssets/LauncherAssets.h"
#include "Launcher/LauncherData/LauncherData.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/ProjectCreator/ProjectCreator.h"
#include "Launcher/ProjectLauncher.h"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.h"
#include "Launcher/SavedProjects/IO/SavedProjectsIO.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/SavedProjects/SavedProjects.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Launcher/SavedProjects/Sorter/SavedProjectsSorter.h"
#include "Launcher/View/ButtonsView/ButtonsView.h"
#include "Launcher/View/ButtonsView/IButtonsView.h"
#include "Launcher/View/ChangeGraphicsApiModal/ChangeGraphicsApiModal.h"
#include "Launcher/View/ChangeGraphicsApiModal/IChangeGraphicsApiModal.h"
#include "Launcher/View/CreateNewProjectModal/CreateNewProjectModal.h"
#include "Launcher/View/CreateNewProjectModal/ICreateNewProjectModal.h"
#include "Launcher/View/FooterView/FooterView.h"
#include "Launcher/View/FooterView/IFooterView.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Launcher/View/ProjectLauncherView.h"
#include "Launcher/View/ProjectListView/IProjectListView.h"
#include "Launcher/View/ProjectListView/ProjectListView.h"
#include "Launcher/View/ProjectNotFoundModal/IProjectNotFoundModal.h"
#include "Launcher/View/ProjectNotFoundModal/ProjectNotFoundModal.h"
#include "Logging/DwarfLogger.h"
#include "Project/ProjectSettingsIO.h"
#include "UI/IImGuiLayerFactory.h"
#include "UI/ImGuiLayerFactory.h"
#include "Utilities/FileHandler/FileHandler.h"
#include "Window/IWindow.h"
#include "Window/SDL3Window.h"

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