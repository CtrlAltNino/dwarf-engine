#pragma once
#include "Utilities/ISerializable.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>

namespace Dwarf
{

  class UUID : public ISerializable
  {
  public:
    UUID()
    {
      boost::uuids::random_generator generator;
      uuid_ = generator();
    }

    UUID(const std::string& serializedUUID) { Deserialize(serializedUUID); }

    std::string
    ToString() const
    {
      return boost::uuids::to_string(uuid_);
    }

    bool
    operator==(const UUID& other) const
    {
      return uuid_ == other.uuid_;
    }

    bool
    operator!=(const UUID& other) const
    {
      return uuid_ != other.uuid_;
    }

    bool
    operator<(const UUID& other) const
    {
      return uuid_ < other.uuid_;
    }

    nlohmann::json
    Serialize() const override
    {
      return boost::uuids::to_string(uuid_);
    }

    void
    Deserialize(const std::string& data)
    {
      uuid_ = boost::uuids::string_generator()(data);
    }

  private:
    boost::uuids::uuid uuid_;
  };
}