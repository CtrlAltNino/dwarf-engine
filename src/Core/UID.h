#pragma once

namespace Dwarf {

    /// @brief Class for 64 bit UIDs.
    class UID {
        public:
            UID();
            UID(uint64_t uuid);
            UID(const UID&) = default;

            operator uint64_t() const { return uuid; }
        private:
            uint64_t uuid;
    };
}

namespace std {
    template<>
    struct hash<Dwarf::UID>{
        std::size_t operator()(const Dwarf::UID& uuid) const {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}