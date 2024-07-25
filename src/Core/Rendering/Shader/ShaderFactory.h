#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Shader/IShader.h"
#include "IShaderFactory.h"

namespace Dwarf
{
  class ShaderFactory : public IShaderFactory
  {
  private:
    GraphicsApi m_GraphicsApi;

  public:
    ShaderFactory(GraphicsApi graphicsApi);
    std::shared_ptr<IShader>
    CreateShader() override;
  };
} // namespace Dwarf