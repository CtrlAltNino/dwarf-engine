#pragma once

#include "Core/Rendering/Shader/IShaderFactory.h"
#include "IShaderRegistry.h"
#include "Logging/IDwarfLogger.h"
namespace Dwarf
{
  class ShaderRegistry : public IShaderRegistry
  {
  private:
    std::shared_ptr<IDwarfLogger>                        m_Logger;
    std::unordered_map<size_t, std::shared_ptr<IShader>> m_Shaders;
    std::shared_ptr<IShaderFactory>                      m_ShaderFactory;

  public:
    ShaderRegistry(std::shared_ptr<IDwarfLogger>   logger,
                   std::shared_ptr<IShaderFactory> shaderFactory);

    ~ShaderRegistry() override;

    std::shared_ptr<IShader>
    GetOrCreate(
      std::unique_ptr<IShaderSourceCollection> shaderSources) override;
  };
}