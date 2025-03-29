#pragma once

#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.h"
#include "IDrawCallWorker.h"

namespace Dwarf
{
  class IDrawCallWorkerFactory
  {
  public:
    virtual ~IDrawCallWorkerFactory() = default;

    [[nodiscard]] virtual auto
    Create(std::unique_ptr<IDrawCallList>& drawCallList) const
      -> std::unique_ptr<IDrawCallWorker> = 0;
  };
}