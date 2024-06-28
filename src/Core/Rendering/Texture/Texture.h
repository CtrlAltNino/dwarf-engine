#pragma once
#include "pch.h"
#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  class Texture
  {
  private:
    glm::ivec3 m_Size;

  public:
    virtual ~Texture() = default;
    // glm::ivec3
    // GetSize() const
    // {
    //   return m_Size;
    // }

    void
    SetSize(glm::ivec3 size)
    {
      m_Size = size;
    }

    static std::shared_ptr<Texture>
    Create(std::shared_ptr<TextureContainer>  data,
           std::shared_ptr<TextureParameters> parameters =
             std::make_shared<TextureParameters>());
    virtual uintptr_t
    GetTextureID() = 0;
  };
} // namespace Dwarf