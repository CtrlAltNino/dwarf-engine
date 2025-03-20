#include "ShaderRecompiler.h"

namespace Dwarf
{
  void
  ShaderRecompiler::MarkForRecompilation(std::shared_ptr<IShader> shader)
  {
    mShadersToRecompile.push_back(shader);
  }

  void
  ShaderRecompiler::Recompile()
  {
    for (const auto& shader : mShadersToRecompile)
    {
      shader->Compile();
    }
    mShadersToRecompile.clear();
  }
}