#ifndef RR99_INPUT_INPUTMANAGER_H
#define RR99_INPUT_INPUTMANAGER_H

#include "InputState.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>

namespace rr99 {

class InputManager {
public:
    InputManager();
    ~InputManager();

    bool initialize(SDL_Window* window);
    void resetState(InputState& state);
    void processEvent(const SDL_Event& event, InputState& state);

    // Key rebinding
    void rebindKey(const std::string& action, SDL_Keycode newKey);
    void rebindGamepadButton(const std::string& action, int button);
    SDL_Keycode getBoundKey(const std::string& action) const;
    int getBoundGamepadButton(const std::string& action) const;

    // Gamepad / haptic
    int getNumGamepads() const;
    void openGamepad(int index);
    void closeGamepad();
    void rumble(float intensity, uint32_t durationMs);

    // Touch input (mobile)
    struct TouchZone {
        SDL_Rect rect;
        std::string action;
    };
    void setTouchLayout(const std::vector<TouchZone>& zones);

private:
    void processKeyEvent(const SDL_Event& event, InputState& state);
    void processGamepadEvent(const SDL_Event& event, InputState& state);
    void processTouchEvent(const SDL_Event& event, InputState& state);

    std::unordered_map<std::string, SDL_Keycode> m_keybindings;
    std::unordered_map<std::string, int> m_gamepadBindings;

    SDL_GameController* m_controller = nullptr;
    SDL_Haptic* m_haptic = nullptr;
    int m_hapticEffectId = -1;

    SDL_Window* m_window = nullptr;
    std::vector<TouchZone> m_touchZones;
    bool m_initialized = false;
};

} // namespace rr99

#endif // RR99_INPUT_INPUTMANAGER_H
