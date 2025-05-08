#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Base.hpp"
#include "Core/Rendering/CubeMapGenerator/ICubeMapGenerator.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/RendererApi/IRendererApiFactory.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "ICubeMapGeneratorFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class CubeMapGeneratorFactory : public ICubeMapGeneratorFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>    mLogger;
    GraphicsApi                      mApi;
    std::shared_ptr<ITextureFactory> mTextureFactory;
    std::shared_ptr<IShaderRegistry> mShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                         mShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>        mMeshFactory;
    std::shared_ptr<IMeshBufferFactory>  mMeshBufferFactory;
    std::shared_ptr<IFramebufferFactory> mFramebufferFactory;
    std::shared_ptr<IRendererApiFactory> mRendererApiFactory;

  public:
    CubeMapGeneratorFactory(
      std::shared_ptr<IDwarfLogger>    logger,
      GraphicsApi                      graphicsApi,
      std::shared_ptr<ITextureFactory> textureFactory,
      std::shared_ptr<IShaderRegistry> shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
                                           shaderSourceCollectionFactory,
      std::shared_ptr<IMeshFactory>        meshFactory,
      std::shared_ptr<IMeshBufferFactory>  meshBufferFactory,
      std::shared_ptr<IFramebufferFactory> framebufferFactory,
      std::shared_ptr<IRendererApiFactory> rendererApiFactory);
    ~CubeMapGeneratorFactory() override = default;

    [[nodiscard]] auto
    Create() -> std::shared_ptr<ICubeMapGenerator> override;
  };
}