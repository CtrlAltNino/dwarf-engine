#pragma once

#include<xhash>
#include<random>

class UID {
    public:
        UID();
        UID(uint64_t uuid);
        UID(const UID&) = default;

        operator uint64_t() const { return uuid; }
    private:
        uint64_t uuid;
};

namespace std {
    template<>
    struct hash<UID>{
        std::size_t operator()(const UID& uuid) const {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}