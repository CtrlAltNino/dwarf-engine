#include "ShaderParameterCollection.h"

namespace Dwarf
{
  ShaderParameterCollection::ShaderParameterCollection() {}

  ShaderParameterCollection::~ShaderParameterCollection() {}

  void
  ShaderParameterCollection::SetParameter(std::string_view identifier,
                                          ParameterValue   parameter)
  {
    mParameters[identifier.data()] = std::move(parameter);
  }

  ParameterValue&
  ShaderParameterCollection::GetParameter(const std::string& name)
  {
    return mParameters.at(name);
  }

  const std::vector<std::string>
  ShaderParameterCollection::GetParameterIdentifiers() const
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
  ShaderParameterCollection::RemoveParameter(std::string const& name)
  {
    mParameters.erase(name);
  }

  bool
  ShaderParameterCollection::HasParameter(std::string const& name) const
  {
    return mParameters.find(name) != mParameters.end();
  }

  void
  ShaderParameterCollection::ClearParameters()
  {
    mParameters.clear();
  }

  nlohmann::json
  ShaderParameterCollection::Serialize()
  {
    nlohmann::json serialized = nlohmann::json::object();
    for (const auto& pair : mParameters)
    {
      std::visit(
        [&serialized, &pair](auto&& arg)
        {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, bool>)
          {
            serialized[pair.first]["type"] = "boolean";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, int>)
          {
            serialized[pair.first]["type"] = "integer";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, unsigned int>)
          {
            serialized[pair.first]["type"] = "unsigned integer";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, float>)
          {
            serialized[pair.first]["type"] = "float";
            serialized[pair.first]["value"] = arg;
          }
          else if constexpr (std::is_same_v<T, glm::vec2>)
          {
            serialized[pair.first]["type"] = "vec2";
            serialized[pair.first]["value"]["x"] = arg.x;
            serialized[pair.first]["value"]["y"] = arg.y;
          }
          else if constexpr (std::is_same_v<T, glm::vec3>)
          {
            serialized[pair.first]["type"] = "vec3";
            serialized[pair.first]["value"]["x"] = arg.x;
            serialized[pair.first]["value"]["y"] = arg.y;
            serialized[pair.first]["value"]["z"] = arg.z;
          }
          else if constexpr (std::is_same_v<T, glm::vec4>)
          {
            serialized[pair.first]["type"] = "vec4";
            serialized[pair.first]["value"]["x"] = arg.x;
            serialized[pair.first]["value"]["y"] = arg.y;
            serialized[pair.first]["value"]["z"] = arg.z;
            serialized[pair.first]["value"]["w"] = arg.w;
          }
          else if constexpr (std::is_same_v<T, Texture2DAssetValue>)
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