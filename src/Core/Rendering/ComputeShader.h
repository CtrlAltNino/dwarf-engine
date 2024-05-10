#pragma once
#include "pch.h"

#include "Core/Base.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
  class ComputeShader
  {
  private:
    bool m_SuccessfullyCompiled;
    std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
      m_Parameters;

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
    virtual std::
      map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
      GetParameters() = 0;
    virtual void
    SetParameters(
      std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
        parameters)
    {
      m_Parameters = parameters;
    }

    virtual void
    UpdateParameters() = 0;

    template<typename T>
    void
    SetParameter(std::string_view identifier, T parameter);

    void
    SetParameter(std::string_view     identifier,
                 std::shared_ptr<UID> value,
                 ShaderParameterType  type);

    static std::shared_ptr<ComputeShader>
    Create();

    static void
    Init();

    static std::shared_ptr<ComputeShader> s_PropagationShader;
    static std::shared_ptr<ComputeShader> s_FinalizationShader;

    static std::filesystem::path
    GetOutlineShaderPath();

    static std::shared_ptr<IShaderParameter>
    CreateComputeShaderParameter(ShaderParameterType type);
  };
}