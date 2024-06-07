#pragma once
#include "pch.h"

#include "Core/Base.h"
#include "Core/UID.h"
#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
  class IComputeShader
  {
  private:
    // Flag to determine if the shader has been successfully compiled.
    bool m_SuccessfullyCompiled;
    // Map of parameters that the shader uses.
    std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
      m_Parameters;

  protected:
    void
    SetIsCompiled(bool compiled)
    {
      m_SuccessfullyCompiled = compiled;
    }

  public:
    virtual ~IComputeShader();

    // Compiles the shader.
    virtual void
    Compile() = 0;

    /**
     * @brief Returns the compilation status of the shader.
     *
     * @return true if the shader has been compiled, false otherwise.
     */
    virtual bool
    IsCompiled() const = 0;

    /**
     * @brief Sends the parameters to the shader.
     */
    virtual void
    UpdateParameters() = 0;

    /**
     * @brief Sets the parameters of the shader.
     */
    void
    SetParameters(
      std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
        parameters)
    {
      m_Parameters = parameters;
    }

    /**
     * @brief Sets a parameter to a direct Value.
     */
    template<typename T>
    void
    SetParameter(std::string_view identifier, T parameter);

    /**
     * @brief Sets a parameter to an AssetReference.
     */
    void
    SetParameter(std::string_view     identifier,
                 std::shared_ptr<UID> value,
                 ShaderParameterType  type);
  };
}