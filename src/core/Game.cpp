#include "Game.h"

#include "EventBus.h"
#include "GameState.h"
#include "InputManager.h"
#include "InputState.h"
#include "Renderer.h"

#include <SDL2/SDL.h>
#include <iostream>

namespace rr99 {

Game::Game()
    : m_renderer(std::make_unique<Renderer>())
    , m_inputManager(std::make_unique<InputManager>())
    , m_eventBus(std::make_unique<EventBus>()) {
}

Game::~Game() {
    m_stateStack.clear();
    SDL_Quit();
}

bool Game::initialize(const std::string& title, int width, int height, bool fullscreen) {
    if (SDL_Init(
            SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_TIMER) <
        0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!m_renderer->initialize(title, width, height, fullscreen)) {
        return false;
    }

    m_renderer->loadFont("resources/OpenSans.ttf");

    m_inputManager->initialize(m_renderer->getWindow());
    m_running = true;
    return true;
}

void Game::run() {
    if (!m_running)
        return;

    Uint64 lastTick = SDL_GetPerformanceCounter();
    const Uint64 freq = SDL_GetPerformanceFrequency();
    float accumulator = 0.0f;

    while (m_running) {
        Uint64 now = SDL_GetPerformanceCounter();
        float frameTime = static_cast<float>(now - lastTick) / static_cast<float>(freq);
        lastTick = now;

        if (frameTime > 0.25f)
            frameTime = 0.25f;

        accumulator += frameTime;

        InputState input;
        m_inputManager->resetState(input);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                m_running = false;
            }

            m_inputManager->processEvent(e, input);

            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    handleResize(e.window.data1, e.window.data2);
                }
                if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    handleFocusChange(true);
                }
                if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    handleFocusChange(false);
                }
            }
        }

        if (input.pause_pressed && !m_stateStack.empty()) {
            m_paused = !m_paused;
        }

        float dt = FIXED_DT;
        if (m_slowMotion) {
            dt *= 0.5f;
        }

        while (!m_paused && m_running && accumulator >= dt) {
            accumulator -= dt;

            if (!m_stateStack.empty()) {
                m_stateStack.back()->handleInput(input);
                m_stateStack.back()->tick(dt);
            }
        }

        m_renderer->beginFrame();
        if (!m_stateStack.empty()) {
            m_stateStack.back()->draw(*m_renderer);
        }
        m_renderer->endFrame();
    }
}

void Game::quit() {
    m_running = false;
}

void Game::pushState(std::unique_ptr<GameState> state) {
    state->enter();
    m_stateStack.push_back(std::move(state));
}

void Game::popState() {
    if (!m_stateStack.empty()) {
        m_stateStack.back()->exit();
        m_stateStack.pop_back();
    }
}

void Game::replaceState(std::unique_ptr<GameState> state) {
    if (!m_stateStack.empty()) {
        m_stateStack.back()->exit();
        m_stateStack.pop_back();
    }
    state->enter();
    m_stateStack.push_back(std::move(state));
}

GameState* Game::getCurrentState() const {
    if (m_stateStack.empty())
        return nullptr;
    return m_stateStack.back().get();
}

void Game::setSlowMotion(bool enabled) {
    m_slowMotion = enabled;
}

void Game::handleResize(int newW, int newH) {
    m_renderer->resize(newW, newH);
}

void Game::handleFocusChange(bool focused) {
    m_focused = focused;
}

} // namespace rr99
