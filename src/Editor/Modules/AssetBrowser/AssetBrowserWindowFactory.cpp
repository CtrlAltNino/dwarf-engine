#include "AssetBrowserWindowFactory.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include <memory>

namespace Dwarf
{
  AssetBrowserWindowFactory::AssetBrowserWindowFactory(
    SerializedModule                  serializedModule,
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IAssetMetadata>   assetMetadata,
    std::shared_ptr<IMaterialCreator> materialCreator)
    : m_InjectorFactory(
        [&serializedModule,
         &assetDirectoryPath,
         &textureFactory,
         &assetDatabase,
         &inputManager,
         &editorSelection,
         &materialIO,
         &materialFactory,
         &assetMetadata,
         &materialCreator]()
        {
          return boost::di::make_injector(
            boost::di::bind<SerializedModule>.to(serializedModule),
            boost::di::bind<AssetDirectoryPath>.to(assetDirectoryPath),
            boost::di::bind<ITextureFactory>.to(textureFactory),
            boost::di::bind<IAssetDatabase>.to(assetDatabase),
            boost::di::bind<IInputManager>.to(inputManager),
            boost::di::bind<IEditorSelection>.to(editorSelection),
            boost::di::bind<IMaterialIO>.to(materialIO),
            boost::di::bind<IMaterialFactory>.to(materialFactory),
            boost::di::bind<IAssetMetadata>.to(assetMetadata),
            boost::di::bind<IMaterialCreator>.to(materialCreator));
        })
  {
  }

  std::shared_ptr<AssetBrowserWindow>
  AssetBrowserWindowFactory::Create() const
  {
    return std::make_shared<AssetBrowserWindow>(
      m_InjectorFactory().create<AssetBrowserWindow>());
  }

  std::shared_ptr<AssetBrowserWindow>
  AssetBrowserWindowFactory::Create(SerializedModule serializedModule) const
  {
    auto injector = boost::di::make_injector(
      m_InjectorFactory(),
      boost::di::bind<SerializedModule>.to(serializedModule));

    return std::make_shared<AssetBrowserWindow>(
      injector.create<AssetBrowserWindow>());
  }
} // namespace Dwarf