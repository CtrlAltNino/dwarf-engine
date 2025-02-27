#pragma once

#include "Utilities/ISerializable.h"
#include "pch.h"
#include "Core/Base.h"
#include "Core/UUID.h"

#include <fmt/base.h>
#include <memory>
#include <variant>
#include <optional>

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
    virtual ParameterValue&
    GetParameter(const std::string& name) = 0;

    /**
     * @brief Gets the list of parameter identifiers.
     */

    virtual const std::vector<std::string>
    GetParameterIdentifiers() const = 0;

    virtual void
    RemoveParameter(const std::string& name) = 0;

    virtual bool
    HasParameter(const std::string& name) const = 0;

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

    virtual nlohmann::json
    Serialize() override = 0;

    std::map<ShaderParameterType, std::function<void(std::string_view)>>
      m_DefaultValueAdders = {
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

    void
    AddDefaultInteger(std::string_view identifier)
    {
      SetParameter(identifier, 0);
    }

    void
    AddDefaultUnsignedInteger(std::string_view identifier)
    {
      SetParameter(identifier, 0u);
    }

    void
    AddDefaultFloat(std::string_view identifier)
    {
      SetParameter(identifier, 0.0f);
    }

    void
    AddDefaultVec2(std::string_view identifier)
    {
      SetParameter(identifier, glm::vec2(0.0f, 0.0f));
    }

    void
    AddDefaultVec3(std::string_view identifier)
    {
      SetParameter(identifier, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void
    AddDefaultVec4(std::string_view identifier)
    {
      SetParameter(identifier, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    }

    void
    AddDefaultBool(std::string_view identifier)
    {
      SetParameter(identifier, false);
    }

    void
    AddDefaultTex2d(std::string_view identifier)
    {
      SetParameter(identifier, std::nullopt);
    }
  };
}