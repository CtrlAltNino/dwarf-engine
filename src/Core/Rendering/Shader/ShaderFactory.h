#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Base.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "IShaderFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class ShaderFactory : public IShaderFactory
  {
  private:
    GraphicsApi                   m_GraphicsApi;
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IShaderSourceCollectionFactory>
      m_ShaderSourceCollectionFactory;
    std::shared_ptr<IShaderParameterCollectionFactory>
                                  m_ShaderParameterCollectionFactory;
    std::shared_ptr<IVramTracker> m_VramTracker;

  public:
    ShaderFactory(GraphicsApi                   graphicsApi,
                  std::shared_ptr<IDwarfLogger> logger,
                  std::shared_ptr<IShaderSourceCollectionFactory>
                    shaderSourceCollectionFactory,
                  std::shared_ptr<IShaderParameterCollectionFactory>
                    shaderParameterCollectionFactory,
                  std::shared_ptr<IVramTracker> vramTracker);
    ~ShaderFactory() override;

    std::shared_ptr<IShader>
    CreateDefaultShader() override;

    std::shared_ptr<IShader>
    CreateErrorShader() override;

    std::shared_ptr<IShader>
    CreateShader(
      std::unique_ptr<IShaderSourceCollection> shaderSources) override;
  };
} // namespace Dwarf