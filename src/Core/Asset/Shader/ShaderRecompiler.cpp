#include "ShaderRecompiler.h"

namespace Dwarf
{
  void
  ShaderRecompiler::MarkForRecompilation(std::shared_ptr<IShader> shader)
  {
    m_ShadersToRecompile.push_back(shader);
  }

  void
  ShaderRecompiler::Recompile()
  {
    for (const auto& shader : m_ShadersToRecompile)
    {
      shader->Compile();
    }
    m_ShadersToRecompile.clear();
  }
}