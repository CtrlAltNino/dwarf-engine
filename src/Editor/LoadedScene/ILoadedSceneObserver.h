#pragma once

namespace Dwarf
{
  class ILoadedSceneObserver
  {
  public:
    virtual ~ILoadedSceneObserver() = default;

    virtual void
    OnSceneLoad() = 0;

    virtual void
    OnSceneUnload() = 0;
  };
}