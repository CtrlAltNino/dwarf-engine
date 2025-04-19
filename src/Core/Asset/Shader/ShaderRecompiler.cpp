#include "pch.hpp"

#include "ShaderRecompiler.hpp"

namespace Dwarf
{
  void
  ShaderRecompiler::MarkForRecompilation(std::shared_ptr<IShader> shader)
  {
    std::unique_lock<std::mutex> lock(mRecompilationMutex);
    mShadersToRecompile.push_back(shader);
  }

  void
  ShaderRecompiler::Recompile()
  {
    std::unique_lock<std::mutex> lock(mRecompilationMutex);
    for (const auto& shader : mShadersToRecompile)
    {
      shader->Compile();
    }
    mShadersToRecompile.clear();
  }
}