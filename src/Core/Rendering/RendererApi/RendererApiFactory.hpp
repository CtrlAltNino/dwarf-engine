#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Base.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/RendererApi/IRendererApiFactory.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Editor/Stats/IEditorStats.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Platform/OpenGL/IOpenGLStateTracker.hpp"
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  class RendererApiFactory : public IRendererApiFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>        mLogger;
    GraphicsApi                          mGraphicsApi;
    std::shared_ptr<IAssetDatabase>      mAssetDatabase;
    std::shared_ptr<IShaderRegistry>     mShaderRegistry;
    std::shared_ptr<IEditorStats>        mEditorStats;
    std::shared_ptr<IOpenGLStateTracker> mStateTracker;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        mShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;

  public:
    RendererApiFactory(std::shared_ptr<IDwarfLogger>        logger,
                       GraphicsApi                          api,
                       std::shared_ptr<IAssetDatabase>      assetDatabase,
                       std::shared_ptr<IShaderRegistry>     shaderRegistry,
                       std::shared_ptr<IEditorStats>        editorStats,
                       std::shared_ptr<IOpenGLStateTracker> stateTracker,
                       std::shared_ptr<IShaderSourceCollectionFactory>
                         shaderSourceCollectionFactory,
                       std::shared_ptr<IMeshFactory>       meshFactory,
                       std::shared_ptr<IMeshBufferFactory> meshBufferFactory);
    ~RendererApiFactory() override;

    /**
     * @brief Creates a renderer api instance
     *
     * @return std::shared_ptr<IRendererApi>
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<IRendererApi> override;
  };
} // namespace Dwarf