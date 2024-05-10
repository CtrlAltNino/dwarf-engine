#pragma once

#include "pch.h"

#include "Core/Base.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
  class Shader
  {
  private:
    bool m_SuccessfullyCompiled;

  public:
    Shader();
    virtual ~Shader();

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

    static std::shared_ptr<Shader>
    Create();

    static void
    Init();

    static std::shared_ptr<Shader> s_DefaultShader;
    static std::shared_ptr<Shader> s_ErrorShader;
    static std::shared_ptr<Shader> s_GridShader;
    static std::shared_ptr<Shader> s_PreviewShader;
    static std::shared_ptr<Shader> s_IdShader;
    static std::shared_ptr<Shader> s_WhiteShader;

    static std::filesystem::path
    GetDefaultShaderPath();
    static std::filesystem::path
    GetErrorShaderPath();
    static std::filesystem::path
    GetGridShaderPath();
    static std::filesystem::path
    GetPreviewShaderPath();
    static std::filesystem::path
    GetIdShaderPath();
    static std::filesystem::path
    GetOutlineShaderPath();

    static std::shared_ptr<IShaderParameter>
    CreateShaderParameter(ShaderParameterType type);
  };
}