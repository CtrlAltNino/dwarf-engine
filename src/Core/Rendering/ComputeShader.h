#pragma once

#include <string>
#include <glm/glm.hpp>
#include <map>

#include "Core/Base.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
  class ComputeShader
  {
  private:
    bool m_SuccessfullyCompiled;
    std::map<std::string, Ref<IShaderParameter>, std::less<>> m_Parameters;

  public:
    ComputeShader();
    virtual ~ComputeShader();

    virtual void
    Compile() = 0;

    bool
    IsCompiled() const
    {
      return m_SuccessfullyCompiled;
    }

    void
    SetIsCompiled(bool isCompiled)
    {
      m_SuccessfullyCompiled = isCompiled;
    }
    virtual std::map<std::string, Ref<IShaderParameter>, std::less<>>
    GetParameters() = 0;
    virtual void
    SetParameters(
      std::map<std::string, Ref<IShaderParameter>, std::less<>> parameters)
    {
      m_Parameters = parameters;
    }

    virtual void
    UpdateParameters() = 0;

    template<typename T>
    void
    SetParameter(std::string_view identifier, T parameter);

    void
    SetParameter(std::string_view    identifier,
                 Ref<UID>            value,
                 ShaderParameterType type);

    static Ref<ComputeShader>
    Create();

    static void
    Init();

    static Ref<ComputeShader> s_PropagationShader;
    static Ref<ComputeShader> s_FinalizationShader;

    static std::filesystem::path
    GetOutlineShaderPath();

    static Ref<IShaderParameter>
    CreateComputeShaderParameter(ShaderParameterType type);
  };
}