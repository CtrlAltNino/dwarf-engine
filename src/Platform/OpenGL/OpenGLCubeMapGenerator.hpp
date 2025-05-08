#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Rendering/CubeMapGenerator/ICubeMapGenerator.hpp"
#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Rendering/RendererApi/IRendererApiFactory.hpp"
#include "Core/Rendering/Shader/IShader.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Core/Rendering/Texture/ITexture.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class OpenGLCubeMapGenerator : public ICubeMapGenerator
  {
  private:
    std::shared_ptr<IDwarfLogger>    mLogger;
    std::shared_ptr<ITextureFactory> mTextureFactory;
    std::shared_ptr<IRendererApi>    mRendererApi;

    std::shared_ptr<IShader>      mConvertShader;
    std::shared_ptr<IMeshBuffer>  mCubeMeshBuffer;
    std::shared_ptr<IFramebuffer> mFramebuffer;

  public:
    OpenGLCubeMapGenerator(
      std::shared_ptr<IDwarfLogger>           logger,
      std::shared_ptr<ITextureFactory>        textureFactory,
      const std::shared_ptr<IShaderRegistry>& shaderRegistry,
      const std::shared_ptr<IShaderSourceCollectionFactory>&
                                                  shaderSourceCollectionFactory,
      const std::shared_ptr<IMeshFactory>&        meshFactory,
      const std::shared_ptr<IMeshBufferFactory>&  meshBufferFactory,
      const std::shared_ptr<IFramebufferFactory>  framebufferFactory,
      const std::shared_ptr<IRendererApiFactory>& rendererApiFactory);
    ~OpenGLCubeMapGenerator() override = default;

    [[nodiscard]] auto
    FromEquirectangular(std::shared_ptr<ITexture> texture, uint32_t resolution)
      -> std::shared_ptr<ITexture> override;
  };
}