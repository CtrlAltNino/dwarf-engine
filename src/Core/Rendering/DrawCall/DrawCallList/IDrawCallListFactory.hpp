#pragma once

#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.hpp"

namespace Dwarf
{
  class IDrawCallListFactory
  {
  public:
    virtual ~IDrawCallListFactory() = default;

    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<IDrawCallList> = 0;
  };
}