#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <filesystem>
#include "Core/Base.h"
#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
    class Texture
    {
    private:
        glm::ivec3 m_Size;
        static Ref<Texture> CreateGpuTexture(Ref<TextureParameters> parameters, Ref<TextureData> data);
        static Ref<TextureParameters> GetParameters(std::filesystem::path const &path);

    public:
        virtual ~Texture() = default;
        glm::ivec3 GetSize() const
        {
            return m_Size;
        }

        void SetSize(glm::ivec3 size)
        {
            m_Size = size;
        }

        static Ref<Texture> Create(TextureParameters const &parameters, glm::ivec2 size, void *data);
        static Ref<Texture> Create(TextureParameters const &parameters, glm::ivec2 size);
        static Ref<Texture> Create(std::filesystem::path const &path);

        virtual uintptr_t GetTextureID() = 0;
    };
}