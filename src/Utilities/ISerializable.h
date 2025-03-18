#pragma once
#include <nlohmann/json.hpp>

namespace Dwarf
{
  class ISerializable
  {
  public:
    virtual nlohmann::json
    Serialize() = 0;
  };
}