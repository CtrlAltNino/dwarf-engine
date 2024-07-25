#pragma once

#include "Core/Rendering/Shader/IShader.h"

namespace Dwarf
{
  class IShaderRecompiler
  {
  public:
    virtual ~IShaderRecompiler() = default;

    virtual void
    MarkForRecompilation(std::shared_ptr<IShader> shader) = 0;

    virtual void
    Recompile() = 0;
  };
}