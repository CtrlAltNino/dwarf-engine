#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Rendering/Shader/ShaderFactory.h"
#include "Editor/Stats/IEditorStats.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  class RendererApiFactory : public IRendererApiFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>   m_Logger;
    GraphicsApi                     m_GraphicsApi;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    std::shared_ptr<IShaderFactory> m_ShaderFactory;
    std::shared_ptr<IEditorStats>   m_EditorStats;

  public:
    RendererApiFactory(std::shared_ptr<IDwarfLogger>   logger,
                       GraphicsApi                     api,
                       std::shared_ptr<IAssetDatabase> assetDatabase,
                       std::shared_ptr<IShaderFactory> shaderFactory,
                       std::shared_ptr<IEditorStats>   editorStats);

    std::shared_ptr<IRendererApi>
    Create() override;
  };
} // namespace Dwarf