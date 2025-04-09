#pragma once

#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include <memory>

namespace Dwarf
{
  using ShaderParameterValue = std::variant<float,
                                            glm::vec2,
                                            glm::vec3,
                                            glm::vec4,
                                            int,
                                            glm::ivec2,
                                            glm::ivec3,
                                            glm::ivec4,
                                            uint32_t,
                                            glm::uvec2,
                                            glm::uvec3,
                                            glm::uvec4,
                                            bool,
                                            glm::mat3,
                                            glm::mat4,
                                            std::shared_ptr<ITexture>>;
  /**
   * @brief Class that represents a shader program and provides controls over
   * it.
   *
   */
  class IShader
  {
  public:
    virtual ~IShader() = default;

    /**
     * @brief Compiles the shader program
     *
     */
    virtual void
    Compile() = 0;

    /**
     * @brief Returns the compilation status of the shader.
     *
     * @return true if the shader has been compiled, false otherwise.
     */
    [[nodiscard]] virtual auto
    IsCompiled() const -> bool = 0;

    /**
     * @brief Sets parameter in the shader
     *
     * @param identifier Identifier of the parameter
     * @param parameter Value of the parameter
     */
    virtual void
    SetParameter(std::string identifier, ShaderParameterValue parameter) = 0;

    /**
     * @brief Creates a ShaderParameterCollection that contains all the shader
     * parameters that the shader uses
     *
     * @return Unique pointer to the created ShaderParameterCollection
     */
    virtual auto
    CreateParameters() -> std::unique_ptr<IShaderParameterCollection> = 0;

    virtual auto
    operator<(const IShader& other) const -> bool = 0;
  };
}