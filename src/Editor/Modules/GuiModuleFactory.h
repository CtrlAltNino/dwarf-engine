#pragma once

#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "Editor/Modules/DebugInformation/DebugWindow.h"
#include "Editor/Modules/IGuiModuleFactory.h"
#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include <boost/di.hpp>
#include <functional>
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  class GuiModuleFactory : public IGuiModuleFactory
  {
  private:
    std::function<boost::di::injector<AssetBrowserWindow,
                                      DebugWindow,
                                      PerformanceWindow,
                                      SceneHierarchyWindow,
                                      SceneViewerWindow,
                                      InspectorWindow>()>
      m_CreateInjector;

  public:
    GuiModuleFactory(
      AssetDirectoryPath                         assetDirectoryPath,
      std::shared_ptr<IInputManager>             inputManager,
      std::shared_ptr<ICameraFactory>            cameraFactory,
      std::shared_ptr<IMaterialFactory>          materialFactory,
      std::shared_ptr<IMaterialIO>               materialIO,
      std::shared_ptr<ILoadedScene>              loadedScene,
      std::shared_ptr<IEditorSelection>          editorSelection,
      std::shared_ptr<IAssetMetadata>            assetMetadata,
      std::shared_ptr<IEditorStats>              editorStats,
      std::shared_ptr<ITextureFactory>           textureFactory,
      std::shared_ptr<IFramebufferFactory>       framebufferFactory,
      std::shared_ptr<IAssetDatabase>            assetDatabase,
      std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory,
      std::shared_ptr<IMaterialCreator>          materialCreator,
      std::shared_ptr<IAssetInspector>           assetInspector,
      std::shared_ptr<IEntityInspector>          entityInspector);

    ~GuiModuleFactory() override = default;
    std::shared_ptr<IGuiModule>
    CreateGuiModule(MODULE_TYPE type) override;

    std::shared_ptr<IGuiModule>
    CreateGuiModule(nlohmann::json serializedModule) override;
  };
} // namespace Dwarf