#pragma once

#include "IShaderRecompiler.hpp"
#include <mutex>

namespace Dwarf
{
  class ShaderRecompiler : public IShaderRecompiler
  {
  public:
    ShaderRecompiler() = default;
    ~ShaderRecompiler() = default;

    void
    MarkForRecompilation(std::shared_ptr<IShader> shader) override;

    void
    Recompile() override;

  private:
    std::vector<std::shared_ptr<IShader>> mShadersToRecompile;
    std::mutex                            mRecompilationMutex;
  };
}