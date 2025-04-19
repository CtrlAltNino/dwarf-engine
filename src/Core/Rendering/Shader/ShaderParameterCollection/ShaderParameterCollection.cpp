#include "pch.h"

#include "ShaderParameterCollection.h"
#include "Utilities/JsonHelper/JsonHelper.h"

namespace Dwarf
{
  void
  ShaderParameterCollection::SetParameter(std::string_view       identifier,
                                          MaterialParameterValue parameter)
  {
    mParameters[identifier.data()] = std::move(parameter);
  }

  auto
  ShaderParameterCollection::GetParameter(const std::string& name)
    -> MaterialParameterValue&
  {
    return mParameters.at(name);
  }

  auto
  ShaderParameterCollection::GetParameterIdentifiers() const
    -> const std::vector<std::string>
  {
    std::vector<std::string> keys;
    keys.reserve(mParameters.size());
    for (const auto& pair : mParameters)
    {
      keys.push_back(pair.first);
    }
    return keys;
  }

  void
  ShaderParameterCollection::PatchParameters(
    const std::unique_ptr<IShaderParameterCollection>& parameters)
  {
    auto currentIdentifiers = GetParameterIdentifiers();
    auto newIdentifiers = parameters->GetParameterIdentifiers();

    // Add new parameters
    for (const auto& identifier : newIdentifiers)
    {
      if (!HasParameter(identifier))
      {
        SetParameter(identifier, parameters->GetParameter(identifier));
      }
    }

    // Removing parameters that aren't present anymore
    for (const auto& identifier : currentIdentifiers)
    {
      if (!parameters->HasParameter(identifier))
      {
        RemoveParameter(identifier);
      }
    }
  }

  void
  ShaderParameterCollection::RemoveParameter(std::string const& name)
  {
    mParameters.erase(name);
  }

  auto
  ShaderParameterCollection::HasParameter(std::string const& name) const -> bool
  {
    return mParameters.contains(name);
  }

  void
  ShaderParameterCollection::ClearParameters()
  {
    mParameters.clear();
  }

  auto
  ShaderParameterCollection::Serialize() -> nlohmann::json
  {
    nlohmann::json serialized = nlohmann::json::object();
    for (const auto& pair : mParameters)
    {
      std::visit(
        [&serialized, &pair](auto&& arg)
        {
          using T = std::decay_t<decltype(arg)>;

          if constexpr (std::is_same_v<T, float>)
          {
            serialized[pair.first]["type"] = "float";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec2>)
          {
            serialized[pair.first]["type"] = "vec2";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec3>)
          {
            serialized[pair.first]["type"] = "vec3";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec4>)
          {
            serialized[pair.first]["type"] = "vec4";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, int>)
          {
            serialized[pair.first]["type"] = "integer";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec2>)
          {
            serialized[pair.first]["type"] = "ivec2";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec3>)
          {
            serialized[pair.first]["type"] = "ivec3";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec4>)
          {
            serialized[pair.first]["type"] = "ivec4";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, uint32_t>)
          {
            serialized[pair.first]["type"] = "unsigned integer";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec2>)
          {
            serialized[pair.first]["type"] = "uvec2";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec3>)
          {
            serialized[pair.first]["type"] = "uvec3";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec4>)
          {
            serialized[pair.first]["type"] = "uvec4";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, bool>)
          {
            serialized[pair.first]["type"] = "boolean";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, std::optional<UUID>>)
          {
            serialized[pair.first]["type"] = "tex2d";
            serialized[pair.first]["value"] =
              arg.has_value() ? arg->toString() : "";
          }
        },
        pair.second);
    }
    return serialized;
  }
}