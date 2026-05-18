#include "InputManager.h"
#include <iostream>

namespace rr99 {

InputManager::InputManager() {
    m_keybindings["accelerate"] = SDLK_LEFT;
    m_keybindings["brake"] = SDLK_RIGHT;
    m_keybindings["pause"] = SDLK_ESCAPE;
    m_keybindings["menu_up"] = SDLK_UP;
    m_keybindings["menu_down"] = SDLK_DOWN;
    m_keybindings["menu_confirm"] = SDLK_RETURN;
    m_keybindings["menu_cancel"] = SDLK_BACKSPACE;

    m_gamepadBindings["accelerate"] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
    m_gamepadBindings["brake"] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
    m_gamepadBindings["pause"] = SDL_CONTROLLER_BUTTON_START;
    m_gamepadBindings["menu_up"] = SDL_CONTROLLER_BUTTON_DPAD_UP;
    m_gamepadBindings["menu_down"] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
    m_gamepadBindings["menu_confirm"] = SDL_CONTROLLER_BUTTON_A;
    m_gamepadBindings["menu_cancel"] = SDL_CONTROLLER_BUTTON_B;
}

InputManager::~InputManager() {
    closeGamepad();
}

bool InputManager::initialize(SDL_Window* window) {
    m_window = window;
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0) {
        std::cerr << "Failed to init input subsystems: " << SDL_GetError() << std::endl;
    }
    m_initialized = true;
    return true;
}

void InputManager::resetState(InputState& state) {
    state.reset();
}

void InputManager::processEvent(const SDL_Event& event, InputState& state) {
    switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            processKeyEvent(event, state);
            break;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            processGamepadEvent(event, state);
            break;
        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
            processTouchEvent(event, state);
            break;
        default:
            break;
    }
}

void InputManager::processKeyEvent(const SDL_Event& event, InputState& state) {
    bool pressed = (event.type == SDL_KEYDOWN);
    SDL_Keycode key = event.key.keysym.sym;

    if (key == m_keybindings["accelerate"]) state.accelerating = pressed;
    if (key == m_keybindings["brake"]) state.braking = pressed;
    if (key == m_keybindings["pause"] && pressed) state.pause_pressed = true;
    if (key == m_keybindings["menu_up"] && pressed) state.menu_up = true;
    if (key == m_keybindings["menu_down"] && pressed) state.menu_down = true;
    if (key == m_keybindings["menu_confirm"] && pressed) state.menu_confirm = true;
    if (key == m_keybindings["menu_cancel"] && pressed) state.menu_cancel = true;
}

void InputManager::processGamepadEvent(const SDL_Event& event, InputState& state) {
    if (!m_controller) return;

    bool pressed = (event.type == SDL_CONTROLLERBUTTONDOWN);
    int button = event.cbutton.button;

    if (button == m_gamepadBindings["accelerate"]) state.accelerating = pressed;
    if (button == m_gamepadBindings["brake"]) state.braking = pressed;
    if (button == m_gamepadBindings["pause"] && pressed) state.pause_pressed = true;
    if (button == m_gamepadBindings["menu_up"] && pressed) state.menu_up = true;
    if (button == m_gamepadBindings["menu_down"] && pressed) state.menu_down = true;
    if (button == m_gamepadBindings["menu_confirm"] && pressed) state.menu_confirm = true;
    if (button == m_gamepadBindings["menu_cancel"] && pressed) state.menu_cancel = true;
}

void InputManager::processTouchEvent(const SDL_Event& event, InputState& state) {
    float x = event.tfinger.x;
    float y = event.tfinger.y;

    int screenW = 0, screenH = 0;
    if (m_window) SDL_GetWindowSize(m_window, &screenW, &screenH);
    int tx = static_cast<int>(x * screenW);
    int ty = static_cast<int>(y * screenH);

    for (const auto& zone : m_touchZones) {
        if (tx >= zone.rect.x && tx <= zone.rect.x + zone.rect.w &&
            ty >= zone.rect.y && ty <= zone.rect.y + zone.rect.h) {
            if (zone.action == "accelerate") state.accelerating = (event.type == SDL_FINGERDOWN);
            if (zone.action == "brake") state.braking = (event.type == SDL_FINGERDOWN);
        }
    }
}

void InputManager::rebindKey(const std::string& action, SDL_Keycode newKey) {
    m_keybindings[action] = newKey;
}

void InputManager::rebindGamepadButton(const std::string& action, int button) {
    m_gamepadBindings[action] = button;
}

SDL_Keycode InputManager::getBoundKey(const std::string& action) const {
    auto it = m_keybindings.find(action);
    return it != m_keybindings.end() ? it->second : SDLK_UNKNOWN;
}

int InputManager::getBoundGamepadButton(const std::string& action) const {
    auto it = m_gamepadBindings.find(action);
    return it != m_gamepadBindings.end() ? it->second : -1;
}

int InputManager::getNumGamepads() const {
    return SDL_NumJoysticks();
}

void InputManager::openGamepad(int index) {
    closeGamepad();
    m_controller = SDL_GameControllerOpen(index);
    if (m_controller) {
        SDL_Joystick* j = SDL_GameControllerGetJoystick(m_controller);
        m_haptic = SDL_HapticOpenFromJoystick(j);
        if (m_haptic) {
            if (SDL_HapticRumbleSupported(m_haptic)) {
                SDL_HapticRumbleInit(m_haptic);
            }
        }
    }
}

void InputManager::closeGamepad() {
    if (m_haptic) {
        SDL_HapticClose(m_haptic);
        m_haptic = nullptr;
    }
    if (m_controller) {
        SDL_GameControllerClose(m_controller);
        m_controller = nullptr;
    }
}

void InputManager::rumble(float intensity, uint32_t durationMs) {
    if (m_haptic && SDL_HapticRumbleSupported(m_haptic)) {
        SDL_HapticRumblePlay(m_haptic, intensity, durationMs);
    }
}

void InputManager::setTouchLayout(const std::vector<TouchZone>& zones) {
    m_touchZones = zones;
}

} // namespace rr99
