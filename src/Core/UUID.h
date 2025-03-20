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
  private:
    boost::uuids::uuid mUuid;

  public:
    UUID()
    {
      boost::uuids::random_generator generator;
      mUuid = generator();
    }

    UUID(const std::string& serializedUUID) { deserialize(serializedUUID); }

    [[nodiscard]] auto
    toString() const -> std::string
    {
      return boost::uuids::to_string(mUuid);
    }

    auto
    operator==(const UUID& other) const -> bool
    {
      return mUuid == other.mUuid;
    }

    auto
    operator!=(const UUID& other) const -> bool
    {
      return mUuid != other.mUuid;
    }

    auto
    operator<(const UUID& other) const -> bool
    {
      return mUuid < other.mUuid;
    }

    auto
    Serialize() -> nlohmann::json override
    {
      return boost::uuids::to_string(mUuid);
    }

    void
    deserialize(const std::string& data)
    {
      mUuid = boost::uuids::string_generator()(data);
    }
  };
}