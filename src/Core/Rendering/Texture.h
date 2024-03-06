#pragma once

#include <glm/vec2.hpp>
#include <filesystem>
#include "Core/Base.h"

namespace Dwarf
{
    class Texture
    {
    private:
        glm::ivec2 m_Size;

    public:
        virtual ~Texture() = default;
        glm::ivec2 GetSize() const
        {
            return m_Size;
        }

        void SetSize(glm::ivec2 size)
        {
            m_Size = size;
        }

        static Ref<Texture> Create(std::filesystem::path const &path);

        virtual uintptr_t GetTextureID() = 0;
    };
}