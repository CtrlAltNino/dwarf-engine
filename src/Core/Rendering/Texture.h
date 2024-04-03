#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <filesystem>
#include "Core/Base.h"

namespace Dwarf
{
    enum class TextureFormat
    {
        RED,
        RG,
        RGB,
        RGBA
    };

    enum class TextureWrap
    {
        UNSET,
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    enum class TextureMinFilter
    {
        UNSET,
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR
    };

    enum class TextureMagFilter
    {
        UNSET,
        NEAREST,
        LINEAR
    };

    enum class TextureType
    {
        TEXTURE_1D,
        TEXTURE_2D,
        TEXTURE_3D,
        TEXTURE_CUBE_MAP
    };

    enum class TextureDataType
    {
        UNSIGNED_BYTE,
        FLOAT,
        UNSIGNED_INT,
        INT
    };

    struct TextureParameters
    {
        TextureType Type = TextureType::TEXTURE_2D;
        TextureFormat Format = TextureFormat::RGBA;
        TextureDataType DataType = TextureDataType::UNSIGNED_BYTE;
        TextureWrap WrapS = TextureWrap::UNSET;
        TextureWrap WrapT = TextureWrap::UNSET;
        TextureWrap WrapR = TextureWrap::UNSET;
        TextureMinFilter MinFilter = TextureMinFilter::UNSET;
        TextureMagFilter MagFilter = TextureMagFilter::UNSET;
    };

    class Texture
    {
    private:
        glm::ivec3 m_Size;
        static Ref<Texture> LoadFromJpg(std::filesystem::path const &path);
        static Ref<Texture> LoadFromPng(std::filesystem::path const &path);
        static Ref<Texture> LoadFromBmp(std::filesystem::path const &path);
        static Ref<Texture> LoadFromTga(std::filesystem::path const &path);
        static Ref<Texture> LoadFromHdr(std::filesystem::path const &path);
        static Ref<Texture> LoadFromTiff(std::filesystem::path const &path);
        static Ref<Texture> CreateGpuTexture(TextureParameters const &parameters, glm::ivec3 size, void *data);

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