#pragma once

#include "IDrawCallListFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class DrawCallListFactory : public IDrawCallListFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    DrawCallListFactory(std::shared_ptr<IDwarfLogger> logger);
    ~DrawCallListFactory() override;

    [[nodiscard]] auto
    Create() const -> std::unique_ptr<IDrawCallList> override;
  };
}