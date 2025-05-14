#pragma once

#include <nlohmann/json.hpp>

namespace Dwarf
{
  class ISerializable
  {
  public:
    virtual ~ISerializable() = default;

    virtual auto
    Serialize() -> nlohmann::json = 0;
  };
}