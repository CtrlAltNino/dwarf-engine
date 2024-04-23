#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Framebuffer.h"
#include "Core/Rendering/Texture.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <filesystem>

namespace Dwarf
{
  class TextureCreator
  {
  private:
    static Ref<TextureParameters>
    GetParameters(std::filesystem::path const& path);
    static Ref<Texture>
    FromPng(std::filesystem::path const& path,
            Ref<TextureParameters>       parameters);
    static Ref<Texture>
    FromJpeg(std::filesystem::path const& path,
             Ref<TextureParameters>       parameters);
    static Ref<Texture>
    FromBmp(std::filesystem::path const& path,
            Ref<TextureParameters>       parameters);
    static Ref<Texture>
    FromTga(std::filesystem::path const& path,
            Ref<TextureParameters>       parameters);
    static Ref<Texture>
    FromHdr(std::filesystem::path const& path,
            Ref<TextureParameters>       parameters);
    static Ref<Texture>
    FromTiff(std::filesystem::path const& path,
             Ref<TextureParameters>       parameters);

  public:
    static Ref<Texture>
    FromData(TextureParameters const& parameters, glm::ivec2 size, void* data);
    static Ref<Texture>
    Empty(glm::ivec2 size);
    static Ref<Texture>
    Empty(FramebufferTextureSpecification const& parameters,
          glm::ivec2                             size,
          int                                    samples = 1);
    static Ref<Texture>
    FromPath(std::filesystem::path const& path);
  };
}