#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Base.hpp"
#include "Core/Rendering/Shader/IShader.hpp"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
#include "IShaderFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class ShaderFactory : public IShaderFactory
  {
  private:
    GraphicsApi                   mGraphicsApi;
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IShaderSourceCollectionFactory>
      mShaderSourceCollectionFactory;
    std::shared_ptr<IShaderParameterCollectionFactory>
                                  mShaderParameterCollectionFactory;
    std::shared_ptr<IVramTracker> mVramTracker;

  public:
    ShaderFactory(GraphicsApi                   graphicsApi,
                  std::shared_ptr<IDwarfLogger> logger,
                  std::shared_ptr<IShaderSourceCollectionFactory>
                    shaderSourceCollectionFactory,
                  std::shared_ptr<IShaderParameterCollectionFactory>
                    shaderParameterCollectionFactory,
                  std::shared_ptr<IVramTracker> vramTracker);
    ~ShaderFactory() override;

    /**
     * @brief Creates a shader instance bases on provided shader sources
     *
     * @param shaderSources Shader sources to create the shader from
     * @return Shared pointer to the created shader
     */
    [[nodiscard]] auto
    Create(std::unique_ptr<IShaderSourceCollection> shaderSources) const
      -> std::shared_ptr<IShader> override;
  };
} // namespace Dwarf