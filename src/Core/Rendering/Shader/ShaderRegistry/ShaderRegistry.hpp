#pragma once

#include "Core/Rendering/Shader/IShaderFactory.hpp"
#include "IShaderRegistry.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class ShaderRegistry : public IShaderRegistry
  {
  private:
    std::shared_ptr<IDwarfLogger>                        mLogger;
    std::shared_ptr<IShaderFactory>                      mShaderFactory;
    std::unordered_map<size_t, std::shared_ptr<IShader>> mShaders;

  public:
    ShaderRegistry(std::shared_ptr<IDwarfLogger>   logger,
                   std::shared_ptr<IShaderFactory> shaderFactory);

    ~ShaderRegistry() override;

    /**
     * @brief Gets or creates a shader program based on source files
     *
     * @param shaderSources Source files to create the shader program from
     * @return A shared pointer to the shader program
     */
    auto
    GetOrCreate(std::unique_ptr<IShaderSourceCollection> shaderSources)
      -> std::shared_ptr<IShader> override;
  };
}