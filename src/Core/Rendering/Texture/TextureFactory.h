#pragma once

#include "Core/Base.h"
#include "ITextureFactory.h"
#include "Core/Asset/Texture/ITextureLoader.h"

namespace Dwarf
{
  class TextureFactory : public ITextureFactory
  {
  private:
    GraphicsApi                     m_Api;
    std::shared_ptr<ITextureLoader> m_Loader;

    std::shared_ptr<ITexture>
    LoadTexture(std::shared_ptr<TextureContainer>  textureData,
                std::shared_ptr<TextureParameters> parameters);

    std::shared_ptr<TextureParameters>
    GetParameters(std::filesystem::path const& path);

  public:
    TextureFactory(GraphicsApi api, std::shared_ptr<ITextureLoader> loader);

    std::shared_ptr<ITexture>
    CreateTexture(std::filesystem::path texturePath) override;
  };
} // namespace Dwarf