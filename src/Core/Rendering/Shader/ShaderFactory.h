#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "IShaderFactory.h"

namespace Dwarf
{
  class ShaderFactory : public IShaderFactory
  {
  private:
    GraphicsApi                     m_GraphicsApi;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;

  public:
    ShaderFactory(GraphicsApi                     graphicsApi,
                  std::shared_ptr<IAssetDatabase> assetDatabase);

    std::shared_ptr<IShader>
    CreateShader() override;

    std::shared_ptr<IShader>
    CreateShader(ShaderSourceCollection shaderSources) override;

    std::shared_ptr<IShader>
    CreateShader(const nlohmann::json& serializedShader) override;
  };
} // namespace Dwarf