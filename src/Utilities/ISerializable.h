#pragma once
#include <nlohmann/json.hpp>

namespace Dwarf
{
  class ISerializable
  {
  public:
    virtual auto
    Serialize() -> nlohmann::json = 0;
  };
}