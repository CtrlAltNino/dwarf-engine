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
    for (auto const& parameter : serializedShaderParameterCollection.items())
    {
      if (serializedShaderParameterCollection[parameter.key()]["type"] ==
          "boolean")
      {
        parameterCollection->SetParameter(parameter.key(),
                                          bool(parameter.value()["value"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "integer")
      {
        parameterCollection->SetParameter(parameter.key(),
                                          int(parameter.value()["value"]));
      }
      else if (serializedShaderParameterCollection[parameter.key()]["type"] ==
               "float")
      {
        parameterCollection->SetParameter(parameter.key(),
                                          float(parameter.value()["value"]));
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
    }
    return parameterCollection;
  }
} // namespace Dwarf