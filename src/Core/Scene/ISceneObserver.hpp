#pragma once

namespace Dwarf
{
  class ISceneObserver
  {
  public:
    virtual void
    OnEntityCreated() = 0;

    virtual void
    OnEntityDeleted() = 0;
  };
}