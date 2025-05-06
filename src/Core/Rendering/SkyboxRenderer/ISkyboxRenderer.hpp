#pragma once

#include "Core/Rendering/Material/IMaterial.hpp"
#include "Core/Rendering/Texture/ITexture.hpp"
#include "Core/Scene/Camera/ICamera.hpp"
namespace Dwarf
{
  class ISkyboxRenderer
  {
  public:
    virtual ~ISkyboxRenderer() = default;

    virtual void
    SetCamera(ICamera& camera) = 0;

    virtual void
    Render() = 0;
  };
}