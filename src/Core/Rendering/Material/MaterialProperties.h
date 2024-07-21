#pragma once

#include "IMaterialProperties.h"
#include <boost/di.hpp>
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(bool, MaterialIsTransparent);
  BOOST_STRONG_TYPEDEF(bool, MaterialIsDoubleSided);
  BOOST_STRONG_TYPEDEF(bool, MaterialIsUnlit);
  BOOST_STRONG_TYPEDEF(bool, MaterialIsWireframe);
  class MaterialProperties : public IMaterialProperties
  {
  public:
    BOOST_DI_INJECT(MaterialProperties,
                    MaterialIsTransparent isTransparent,
                    MaterialIsDoubleSided isDoubleSided,
                    MaterialIsUnlit       isUnlit,
                    MaterialIsWireframe   isWireframe);
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