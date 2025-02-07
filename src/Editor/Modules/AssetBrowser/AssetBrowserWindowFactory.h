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
    AssetDirectoryPath                m_AssetDirectoryPath;
    std::shared_ptr<ITextureFactory>  m_TextureFactory;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<IEditorSelection> m_EditorSelection;
    std::shared_ptr<IMaterialIO>      m_MaterialIO;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IAssetMetadata>   m_AssetMetadata;
    std::shared_ptr<IMaterialCreator> m_MaterialCreator;
    std::shared_ptr<IFileHandler>     m_FileHandler;

  public:
    BOOST_DI_INJECT(AssetBrowserWindowFactory,
                    AssetDirectoryPath                assetDirectoryPath,
                    std::shared_ptr<ITextureFactory>  textureFactory,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IInputManager>    inputManager,
                    std::shared_ptr<IEditorSelection> editorSelection,
                    std::shared_ptr<IMaterialIO>      materialIO,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IAssetMetadata>   assetMetadata,
                    std::shared_ptr<IMaterialCreator> materialCreator,
                    std::shared_ptr<IFileHandler>     fileHandler);

    ~AssetBrowserWindowFactory() override = default;
    std::unique_ptr<AssetBrowserWindow>
    Create() const override;

    std::unique_ptr<AssetBrowserWindow>
    Create(SerializedModule serializedModule) const override;
  };
} // namespace Dwarf