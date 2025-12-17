#pragma once

#include <nlohmann/json.hpp>

namespace Dwarf
{
  class ISerializable
  {
  public:
    virtual ~ISerializable() = default;

    /**
     * @brief Returns the serialized state of a class
     *
     * @return nlohmann::json JSON object representing the class state
     */
    virtual auto
    Serialize() -> nlohmann::json = 0;
  };
}