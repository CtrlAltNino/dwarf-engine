#pragma once

#include "Core/Base.h"
#include "Core/UID.h"

namespace Dwarf {
    struct PathComponent {
         /// @brief Path to the asset.
        std::filesystem::path Path;

        PathComponent() = default;
        PathComponent(const PathComponent&) = default;
        PathComponent(const std::filesystem::path& path)
            : Path(path) {}
    };

    /// @brief Entity component holding a unique identifier (UID / GUID).
    struct IDComponent{
        /// @brief The UID of an entity.
        Ref<UID> ID;
        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(const UID& other) : ID(CreateRef<UID>(other)){}
    };

    /// @brief Entity component holding a tag (Used for object names).
    struct TagComponent{
        /// @brief The entitie's tag.
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}
    };
}