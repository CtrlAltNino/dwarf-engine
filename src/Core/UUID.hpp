#pragma once

#include "Utilities/ISerializable.hpp"
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

    UUID(const UUID& other) { deserialize(other.toString()); }

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

namespace std
{
  template<>
  struct hash<Dwarf::UUID>
  {
    size_t
    operator()(const Dwarf::UUID& uuid) const
    {
      // Replace this with your actual hashing logic
      // If UUID has a 'std::string value' inside:
      return std::hash<std::string>{}(uuid.toString());
    }
  };
}