#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Base.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "Editor/Stats/IEditorStats.h"
#include "Logging/IDwarfLogger.h"
#include "Platform/OpenGL/IOpenGLStateTracker.h"
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  class RendererApiFactory : public IRendererApiFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>        m_Logger;
    GraphicsApi                          m_GraphicsApi;
    std::shared_ptr<IAssetDatabase>      m_AssetDatabase;
    std::shared_ptr<IShaderRegistry>     m_ShaderRegistry;
    std::shared_ptr<IEditorStats>        m_EditorStats;
    std::shared_ptr<IOpenGLStateTracker> m_StateTracker;
    std::shared_ptr<IShaderSourceCollectionFactory>
      m_ShaderSourceCollectionFactory;

  public:
    RendererApiFactory(std::shared_ptr<IDwarfLogger>        logger,
                       GraphicsApi                          api,
                       std::shared_ptr<IAssetDatabase>      assetDatabase,
                       std::shared_ptr<IShaderRegistry>     shaderRegistry,
                       std::shared_ptr<IEditorStats>        editorStats,
                       std::shared_ptr<IOpenGLStateTracker> stateTracker,
                       std::shared_ptr<IShaderSourceCollectionFactory>
                         shaderSourceCollectionFactory);
    ~RendererApiFactory() override;
    std::shared_ptr<IRendererApi>
    Create() override;
  };
} // namespace Dwarf