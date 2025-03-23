#pragma once

#include "Logging/IDwarfLogger.h"
#include "UI/IImGuiLayer.h"
#include <SDL3/SDL_video.h>

namespace Dwarf
{
  class OpenGLImGuiLayer : public IImGuiLayer
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::string                   mIniFilePath;
    SDL_Window*                   mWindow = nullptr;

  public:
    OpenGLImGuiLayer(std::shared_ptr<IDwarfLogger> logger,
                     const ImGuiIniFilePath&       iniFilePath);
    ~OpenGLImGuiLayer() override;

    /**
     * @brief Initializes the ImGui context for a given SDL window
     *
     * @param window
     */
    void
    OnAttach(SDL_Window* window) override;

    /**
     * @brief Cleans up the imgui context
     *
     */
    void
    OnDetach() override;

    /**
     * @brief Start of a frame
     *
     */
    void
    Begin() override;

    /**
     * @brief End of a frame
     *
     */
    void
    End() override;

    /**
     * @brief Handles a SDL Event
     *
     * @param event
     */
    void
    HandleSDLEvent(SDL_Event* event) override;
  };
}