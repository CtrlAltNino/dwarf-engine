#include "MaterialProperties.h"

namespace Dwarf
{
  MaterialProperties::MaterialProperties(bool isTransparent,
                                         bool isDoubleSided,
                                         bool isUnlit,
                                         bool isWireframe)
    : m_IsTransparent(isTransparent)
    , m_IsDoubleSided(isDoubleSided)
    , m_IsUnlit(isUnlit)
    , m_IsWireframe(isWireframe)
  {
  }

  bool
  MaterialProperties::IsTransparent() const
  {
    return m_IsTransparent;
  }

  bool
  MaterialProperties::IsDoubleSided() const
  {
    return m_IsDoubleSided;
  }

  bool
  MaterialProperties::IsUnlit() const
  {
    return m_IsUnlit;
  }

  bool
  MaterialProperties::IsWireframe() const
  {
    return m_IsWireframe;
  }
}