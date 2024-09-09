#pragma once

#include <boost/di.hpp>
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "Editor/Modules/AssetBrowser/IAssetBrowserWindowFactory.h"

namespace Dwarf
{
  class AssetBrowserWindowFactory : public IAssetBrowserWindowFactory
  {
  private:
    std::function<boost::di::injector<std::unique_ptr<AssetBrowserWindow>>()>
      m_InjectorFactory;

  public:
    BOOST_DI_INJECT(AssetBrowserWindowFactory,
                    SerializedModule                  serializedModule,
                    AssetDirectoryPath                assetDirectoryPath,
                    std::shared_ptr<ITextureFactory>  textureFactory,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IInputManager>    inputManager,
                    std::shared_ptr<IEditorSelection> editorSelection,
                    std::shared_ptr<IMaterialIO>      materialIO,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IAssetMetadata>   assetMetadata,
                    std::shared_ptr<IMaterialCreator> materialCreator);

    ~AssetBrowserWindowFactory() override = default;
    std::unique_ptr<AssetBrowserWindow>
    Create() const override;

    std::unique_ptr<AssetBrowserWindow>
    Create(SerializedModule serializedModule) const override;
  };
} // namespace Dwarf