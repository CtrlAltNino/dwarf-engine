#include "pch.h"

#include "ShaderParameterCollection.h"
#include "ShaderParameterCollectionFactory.h"

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
        parameterCollection->SetParameter(parameter.key(),
                                          float(parameter.value()["value"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "vec2")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::vec2((float)parameter.value()["value"]["x"],
                    (float)parameter.value()["value"]["y"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "vec3")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::vec3((float)parameter.value()["value"]["x"],
                    (float)parameter.value()["value"]["y"],
                    (float)parameter.value()["value"]["z"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "vec4")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::vec4((float)parameter.value()["value"]["x"],
                    (float)parameter.value()["value"]["y"],
                    (float)parameter.value()["value"]["z"],
                    (float)parameter.value()["value"]["w"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "integer")
      {
        parameterCollection->SetParameter(parameter.key(),
                                          int(parameter.value()["value"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "ivec2")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::ivec2((int)parameter.value()["value"]["x"],
                     (int)parameter.value()["value"]["y"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "ivec3")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::ivec3((int)parameter.value()["value"]["x"],
                     (int)parameter.value()["value"]["y"],
                     (int)parameter.value()["value"]["z"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "vec4")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::ivec4((int)parameter.value()["value"]["x"],
                     (int)parameter.value()["value"]["y"],
                     (int)parameter.value()["value"]["z"],
                     (int)parameter.value()["value"]["w"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "unsigned integer")
      {
        parameterCollection->SetParameter(parameter.key(),
                                          int(parameter.value()["value"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "uvec2")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::uvec2((uint32_t)parameter.value()["value"]["x"],
                     (uint32_t)parameter.value()["value"]["y"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "uvec3")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::uvec3((uint32_t)parameter.value()["value"]["x"],
                     (uint32_t)parameter.value()["value"]["y"],
                     (uint32_t)parameter.value()["value"]["z"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "uvec4")
      {
        parameterCollection->SetParameter(
          parameter.key(),
          glm::uvec4((uint32_t)parameter.value()["value"]["x"],
                     (uint32_t)parameter.value()["value"]["y"],
                     (uint32_t)parameter.value()["value"]["z"],
                     (uint32_t)parameter.value()["value"]["w"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "boolean")
      {
        parameterCollection->SetParameter(parameter.key(),
                                          bool(parameter.value()["value"]));
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