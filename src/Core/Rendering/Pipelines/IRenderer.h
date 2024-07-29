#pragma once

namespace Dwarf
{
  enum class RendererType
  {
    Forward,
    Deferred
  };
  class IRenderer
  {
  public:
    virtual ~IRenderer() = default;
    virtual void
    Render() = 0;
  };
}