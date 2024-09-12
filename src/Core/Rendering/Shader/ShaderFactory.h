#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Asset/Shader/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "IShaderFactory.h"

namespace Dwarf
{
  class ShaderFactory : public IShaderFactory
  {
  private:
    GraphicsApi m_GraphicsApi;
    std::shared_ptr<IShaderSourceCollectionFactory>
      m_ShaderSourceCollectionFactory;
    std::shared_ptr<IShaderParameterCollectionFactory>
      m_ShaderParameterCollectionFactory;

  public:
    ShaderFactory(GraphicsApi graphicsApi,
                  std::shared_ptr<IShaderSourceCollectionFactory>
                    shaderSourceCollectionFactory,
                  std::shared_ptr<IShaderParameterCollectionFactory>
                    shaderParameterCollectionFactory);
    ~ShaderFactory() = default;

    std::unique_ptr<IShader>
    CreateShader() override;

    std::unique_ptr<IShader>
    CreateShader(ShaderSourceCollection& shaderSources) override;

    std::unique_ptr<IShader>
    CreateShader(const nlohmann::json& serializedShader) override;
  };
} // namespace Dwarf