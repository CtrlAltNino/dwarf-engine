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
    boost::uuids::uuid m_Uuid;

  public:
    UUID()
    {
      boost::uuids::random_generator generator;
      m_Uuid = generator();
    }

    UUID(const std::string& serializedUUID) { deserialize(serializedUUID); }

    [[nodiscard]] auto
    toString() const -> std::string
    {
      return boost::uuids::to_string(m_Uuid);
    }

    auto
    operator==(const UUID& other) const -> bool
    {
      return m_Uuid == other.m_Uuid;
    }

    auto
    operator!=(const UUID& other) const -> bool
    {
      return m_Uuid != other.m_Uuid;
    }

    auto
    operator<(const UUID& other) const -> bool
    {
      return m_Uuid < other.m_Uuid;
    }

    auto
    Serialize() -> nlohmann::json override
    {
      return boost::uuids::to_string(m_Uuid);
    }

    void
    deserialize(const std::string& data)
    {
      m_Uuid = boost::uuids::string_generator()(data);
    }
  };
}