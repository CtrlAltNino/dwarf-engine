#pragma once

namespace Dwarf
{
  class ISerializable
  {
  public:
    virtual nlohmann::json
    Serialize() = 0;
  };
}