#pragma once
#include "Input/IInputManager.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class InputManager : public IInputManager
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    /**
     * Saved position of the current mouse position
     */
    glm::vec2 mCurrentMousePos = glm::vec2(0);
    glm::vec2 mLastMousePos = glm::vec2(0);
    glm::vec2 mDeltaMousePos = glm::vec2(0);
    glm::vec2 mDeltaScroll = glm::vec2(0);

    /**
     * State of the mouse buttons
     */
    std::map<MOUSE_BUTTON, int> mMouseButtonStates;

    /**
     * Mapping the engine specific key codes to the SDL3 codes
     */
    static std::map<SDL_Scancode, KEYCODE> sKeyCodeMap;
    std::set<KEYCODE>                      mKeysDown;
    std::set<KEYCODE>                      mKeysRepeat;
    std::set<KEYCODE>                      mKeysUp;

    /**
     * Mapping the engine specific key codes to the SDL3 codes
     */
    static std::map<MOUSE_BUTTON, int> sMouseCodeMap;

  public:
    InputManager(std::shared_ptr<IDwarfLogger> logger);
    ~InputManager() override;
    /**
     * Returns true while the specified key is being pressed
     */
    [[nodiscard]] auto
    GetKey(KEYCODE key) const -> bool override;
    /**
     * Returns true during the frame the specified key starts being pressed
     */
    [[nodiscard]] auto
    GetKeyDown(KEYCODE key) const -> bool override;
    /**
     * Return true during the frame the specified key is being released
     */
    [[nodiscard]] auto
    GetKeyUp(KEYCODE key) const -> bool override;

    /**
     * Returns true while the specified mouse button is being pressed
     */
    [[nodiscard]] auto
    GetMouseButton(MOUSE_BUTTON button) const -> bool override;
    /**j
     * Returns true during the frame the specified mouse button starts being
     * pressed
     */
    [[nodiscard]] auto
    GetMouseButtonDown(MOUSE_BUTTON button) const -> bool override;
    /**j
     * Return true during the frame the specified mouse button is being released
     */
    [[nodiscard]] auto
    GetMouseButtonUp(MOUSE_BUTTON button) const -> bool override;

    /**
     * Returns the current position of the cursor in a vector
     */
    [[nodiscard]] auto
    GetMousePosition() const -> glm::vec2 override;

    // void
    // SetDeltaMousePos(float x, float y);

    [[nodiscard]] auto
    GetMouseDelta() const -> glm::vec2 override;

    [[nodiscard]] auto
    GetMouseScrollDelta() const -> glm::vec2 override;

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