#pragma once

namespace Dwarf {

  /// @brief Class for 64 bit UIDs.
  class UID
  {
  public:
    UID();
    explicit UID(uint64_t uuid);
    UID(const UID&) = default;

    operator uint64_t() const { return m_Uuid; }

  private:
    uint64_t m_Uuid;
  };
}

namespace std {
  template<>
  struct hash<Dwarf::UID>
  {
    std::size_t operator()(const Dwarf::UID& uuid) const
    {
      return hash<uint64_t>()((uint64_t)uuid);
    }
  };
}