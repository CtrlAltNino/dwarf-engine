#include "pch.hpp"

#include "ShaderParameterCollection.hpp"
#include "ShaderParameterCollectionFactory.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"
#include <glm/ext/vector_float3.hpp>

namespace Dwarf
{
  ShaderParameterCollectionFactory::ShaderParameterCollectionFactory(
    GraphicsApi graphicsApi)
    : mGraphicsApi(graphicsApi)
  {
  }

  auto
  ShaderParameterCollectionFactory::Create() const
    -> std::unique_ptr<IShaderParameterCollection>
  {
    return std::make_unique<ShaderParameterCollection>();
  }

  auto
  ShaderParameterCollectionFactory::FromSerialized(
    const nlohmann::json& serializedShaderParameterCollection) const
    -> std::unique_ptr<IShaderParameterCollection>
  {
    std::unique_ptr<ShaderParameterCollection> parameterCollection =
      std::make_unique<ShaderParameterCollection>();
    for (const auto& parameter : serializedShaderParameterCollection.items())
    {
      if (serializedShaderParameterCollection[parameter.key()]["type"] ==
          "float")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<float>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "vec2")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::vec2>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "vec3")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::vec3>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "vec4")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::vec4>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "integer")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<int>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "ivec2")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::ivec2>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "ivec3")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::ivec3>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "vec4")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::ivec4>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "unsigned integer")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<unsigned int>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "uvec2")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::uvec2>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "uvec3")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::uvec3>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "uvec4")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<glm::uvec4>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "boolean")
      {
        parameterCollection->SetParameter(
          parameter.key(), parameter.value()["value"].get<bool>());
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "tex2d")
      {
        if (parameter.value()["value"] != "")
        {
          parameterCollection->SetParameter(
            parameter.key(),
            UUID(parameter.value()["value"].get<std::string>()));
        }
        else
        {
          parameterCollection->SetParameter(parameter.key(), std::nullopt);
        }
      }
    }
    return parameterCollection;
  }
} // namespace Dwarf