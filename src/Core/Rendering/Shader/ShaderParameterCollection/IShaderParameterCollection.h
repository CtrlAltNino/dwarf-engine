#pragma once

#include "Core/Base.h"
#include "Core/UUID.h"
#include "Utilities/ISerializable.h"

#include <optional>
#include <variant>

namespace Dwarf
{
  using Texture2DAssetValue = std::optional<UUID>;
  using ParameterValue = std::variant<bool,
                                      int,
                                      unsigned int,
                                      float,
                                      Texture2DAssetValue,
                                      glm::vec2,
                                      glm::vec3,
                                      glm::vec4>;

  /**
   * @brief Class that manages shader parameters and provides controlled access.
   * The structure is expected to be in a key value structure.
   *
   */
  class IShaderParameterCollection : ISerializable
  {
  public:
    virtual ~IShaderParameterCollection() = default;

    /**
     * @brief Sets a parameter to a direct Value.
     */
    virtual void
    SetParameter(std::string_view identifier, ParameterValue parameter) = 0;

    /**
     * @brief Gets a parameter from the collection.
     */
    virtual auto
    GetParameter(const std::string& name) -> ParameterValue& = 0;

    /**
     * @brief Gets the list of parameter identifiers.
     */
    [[nodiscard]] virtual auto
    GetParameterIdentifiers() const -> const std::vector<std::string> = 0;

    /**
     * @brief Removes a parameter
     *
     * @param name Name of the parameters to remove
     */
    virtual void
    RemoveParameter(const std::string& name) = 0;

    /**
     * @brief Checks if a parameter is present
     *
     * @param name Key to check for
     * @return true If the parameter is present
     * @return false If the parameter is not present
     */
    [[nodiscard]] virtual auto
    HasParameter(const std::string& name) const -> bool = 0;

    /**
     * @brief Clears the parameters.
     */
    virtual void
    ClearParameters() = 0;

    /**
     * @brief Serialize the object.
     *
     * @return The serialized object.
     */
    auto
    Serialize() -> nlohmann::json override = 0;

    /**
     * @brief Map to provide a way to add default values for each parameter type
     *
     */
    std::map<ShaderParameterType, std::function<void(std::string_view)>>
      mDefaultValueAdders = {
        { ShaderParameterType::BOOLEAN,
          [this](std::string_view identifier) { AddDefaultBool(identifier); } },
        { ShaderParameterType::INTEGER,
          [this](std::string_view identifier)
          { AddDefaultInteger(identifier); } },
        { ShaderParameterType::UNSIGNED_INTEGER,
          [this](std::string_view identifier)
          { AddDefaultUnsignedInteger(identifier); } },
        { ShaderParameterType::FLOAT,
          [this](std::string_view identifier)
          { AddDefaultFloat(identifier); } },
        { ShaderParameterType::VEC2,
          [this](std::string_view identifier) { AddDefaultVec2(identifier); } },
        { ShaderParameterType::VEC3,
          [this](std::string_view identifier) { AddDefaultVec3(identifier); } },
        { ShaderParameterType::VEC4,
          [this](std::string_view identifier) { AddDefaultVec4(identifier); } },
        { ShaderParameterType::TEX2D,
          [this](std::string_view identifier)
          { AddDefaultTex2d(identifier); } },
      };

    /**
     * @brief Adds a new parameter and sets it to the default value
     *
     * @param identifier Identifier of the parameter
     */
    void
    AddDefaultInteger(std::string_view identifier)
    {
      SetParameter(identifier, 0);
    }

    /**
     * @brief Adds a new parameter and sets it to the default value
     *
     * @param identifier Identifier of the parameter
     */
    void
    AddDefaultUnsignedInteger(std::string_view identifier)
    {
      SetParameter(identifier, 0U);
    }

    /**
     * @brief Adds a new parameter and sets it to the default value
     *
     * @param identifier Identifier of the parameter
     */
    void
    AddDefaultFloat(std::string_view identifier)
    {
      SetParameter(identifier, 0.0F);
    }

    /**
     * @brief Adds a new parameter and sets it to the default value
     *
     * @param identifier Identifier of the parameter
     */
    void
    AddDefaultVec2(std::string_view identifier)
    {
      SetParameter(identifier, glm::vec2(0.0F, 0.0F));
    }

    /**
     * @brief Adds a new parameter and sets it to the default value
     *
     * @param identifier Identifier of the parameter
     */
    void
    AddDefaultVec3(std::string_view identifier)
    {
      SetParameter(identifier, glm::vec3(0.0F, 0.0F, 0.0F));
    }

    /**
     * @brief Adds a new parameter and sets it to the default value
     *
     * @param identifier Identifier of the parameter
     */
    void
    AddDefaultVec4(std::string_view identifier)
    {
      SetParameter(identifier, glm::vec4(1.0F, 1.0F, 1.0F, 1.0F));
    }

    /**
     * @brief Adds a new parameter and sets it to the default value
     *
     * @param identifier Identifier of the parameter
     */
    void
    AddDefaultBool(std::string_view identifier)
    {
      SetParameter(identifier, false);
    }

    /**
     * @brief Adds a new parameter and sets it to the default value
     *
     * @param identifier Identifier of the parameter
     */
    void
    AddDefaultTex2d(std::string_view identifier)
    {
      SetParameter(identifier, std::nullopt);
    }
  };
}