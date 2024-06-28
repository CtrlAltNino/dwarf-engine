#pragma once
#include "pch.h"
#include "Core/Base.h"
#include "Core/Rendering/Framebuffer/Framebuffer.h"
#include "Core/Rendering/Texture/Texture.h"
#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  class TextureCreator
  {
  private:
    static std::shared_ptr<TextureParameters>
    GetParameters(std::filesystem::path const& path);
    static std::shared_ptr<Texture>
    FromPng(std::filesystem::path const&       path,
            std::shared_ptr<TextureParameters> parameters);
    static std::shared_ptr<Texture>
    FromJpeg(std::filesystem::path const&       path,
             std::shared_ptr<TextureParameters> parameters);
    static std::shared_ptr<Texture>
    FromBmp(std::filesystem::path const&       path,
            std::shared_ptr<TextureParameters> parameters);
    static std::shared_ptr<Texture>
    FromTga(std::filesystem::path const&       path,
            std::shared_ptr<TextureParameters> parameters);
    static std::shared_ptr<Texture>
    FromHdr(std::filesystem::path const&       path,
            std::shared_ptr<TextureParameters> parameters);
    static std::shared_ptr<Texture>
    FromTiff(std::filesystem::path const&       path,
             std::shared_ptr<TextureParameters> parameters);

  public:
    static std::shared_ptr<Texture>
    FromData(TextureParameters const& parameters, glm::ivec2 size, void* data);
    static std::shared_ptr<Texture>
    Empty(glm::ivec2 size);
    static std::shared_ptr<Texture>
    Empty(FramebufferTextureSpecification const& parameters,
          glm::ivec2                             size,
          int                                    samples = 1);
    static std::shared_ptr<Texture>
    FromPath(std::filesystem::path const& path);
  };
}