#pragma once

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

    struct TextureData
    {
        int Width;
        int Height;
        int Channels;
        void *Data;
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
}