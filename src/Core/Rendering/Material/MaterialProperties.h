#pragma once

#include "IMaterialProperties.h"

namespace Dwarf
{
  class MaterialProperties : public IMaterialProperties
  {
  public:
    MaterialProperties(bool isTransparent = false,
                       bool isDoubleSided = false,
                       bool isUnlit = false,
                       bool isWireframe = false);
    virtual ~MaterialProperties() = default;

    bool
    IsTransparent() const override;

    bool
    IsDoubleSided() const override;

    bool
    IsUnlit() const override;

    bool
    IsWireframe() const override;

  private:
    bool m_IsTransparent;
    bool m_IsDoubleSided;
    bool m_IsUnlit;
    bool m_IsWireframe;
  };
}