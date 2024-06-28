#pragma once

#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  class IMaterialProperties
  {
  public:
    virtual ~IMaterialProperties() = default;

    virtual bool
    IsTransparent() const = 0;

    virtual bool
    IsDoubleSided() const = 0;

    virtual bool
    IsUnlit() const = 0;

    virtual bool
    IsWireframe() const = 0;
  };
}