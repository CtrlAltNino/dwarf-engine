#pragma once

#include "IShadowMapper.hpp"

namespace Dwarf
{
  class IShadowMapperFactory
  {
  public:
    virtual ~IShadowMapperFactory() = default;

    virtual auto
    Create() -> std::unique_ptr<IShadowMapper> = 0;
  };
}