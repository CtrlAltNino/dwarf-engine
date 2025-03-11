#pragma once
#include "Logging/IDwarfLogger.h"
#include "pch.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class InputManager : public IInputManager
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    /**
     * Saved position of the current mouse position
     */
    glm::vec2 m_CurrentMousePos = glm::vec2(0);
    glm::vec2 m_LastMousePos = glm::vec2(0);
    glm::vec2 m_DeltaMousePos = glm::vec2(0);
    glm::vec2 m_DeltaScroll = glm::vec2(0);

    /**
     * State of the mouse buttons
     */
    std::map<MOUSE_BUTTON, int> m_MouseButtonStates;

    /**
     * Mapping the engine specific key codes to the SDL3 codes
     */
    static std::map<SDL_Scancode, KEYCODE> s_KeyCodeMap;
    std::set<KEYCODE>                      m_KeysDown;
    std::set<KEYCODE>                      m_KeysRepeat;
    std::set<KEYCODE>                      m_KeysUp;

    /**
     * Mapping the engine specific key codes to the SDL3 codes
     */
    static std::map<MOUSE_BUTTON, int> s_MouseCodeMap;

  public:
    InputManager(std::shared_ptr<IDwarfLogger> logger);
    ~InputManager() override;
    /**
     * Returns true while the specified key is being pressed
     */
    bool
    GetKey(KEYCODE key) const override;
    /**
     * Returns true during the frame the specified key starts being pressed
     */
    bool
    GetKeyDown(KEYCODE key) const override;
    /**
     * Return true during the frame the specified key is being released
     */
    bool
    GetKeyUp(KEYCODE key) const override;

    /**
     * Returns true while the specified mouse button is being pressed
     */
    bool
    GetMouseButton(MOUSE_BUTTON button) const override;
    /**
     * Returns true during the frame the specified mouse button starts being
     * pressed
     */
    bool
    GetMouseButtonDown(MOUSE_BUTTON button) const override;
    /**
     * Return true during the frame the specified mouse button is being released
     */
    bool
    GetMouseButtonUp(MOUSE_BUTTON button) const override;

    /**
     * Toggles the visibility of the cursor
     */
    void
    SetMouseVisibility(bool visibilityState) override;

    /**
     * Returns the current position of the cursor in a vector
     */
    glm::vec2
    GetMousePosition() const override;

    // void
    // SetDeltaMousePos(float x, float y);

    glm::vec2
    GetMouseDelta() const override;

    glm::vec2
    GetMouseScrollDelta() const override;

    void
    SetDeltaMousePos(float x, float y) override;
    void
    SetDeltaMouseScroll(float x, float y) override;

    /**
     * Updates the states of the input states.
     * Call this every frame
     */

    void
    OnUpdate() override;

    void
    ProcessKeyDown(SDL_Scancode key) override;

    void
    ProcessKeyUp(SDL_Scancode key) override;

    void
    ProcessScroll(SDL_Event const& event) override;
  };
}