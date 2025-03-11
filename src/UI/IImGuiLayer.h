#pragma once

#include "Core/Base.h"
#include <SDL3/SDL.h>
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(std::filesystem::path, ImGuiIniFilePath)
  class IImGuiLayer
  {
  protected:
    SDL_Window* m_Window;

  public:
    virtual ~IImGuiLayer() = default;

    virtual void
    OnAttach(SDL_Window* window) = 0;

    virtual void
    OnDetach() = 0;

    virtual void
    Begin() = 0;

    virtual void
    End() = 0;

    virtual void
    HandleSDLEvent(SDL_Event* event) = 0;
  };
}