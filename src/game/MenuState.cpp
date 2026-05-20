#include "MenuState.h"

#include "CareerState.h"
#include "GhostRaceState.h"
#include "HotSeatState.h"
#include "SurvivalState.h"
#include "core/Game.h"
#include "input/InputState.h"
#include "render/Renderer.h"

#include <string>

namespace rr99 {

MenuState::MenuState(Game& game)
    : m_game(&game) {
}

void MenuState::enter() {
    m_selectedOption = 0;
    m_animTimer = 0.0f;
}

void MenuState::exit() {
}

void MenuState::tick(float dt) {
    m_animTimer += dt;
}

void MenuState::draw(Renderer& renderer) {
    int w = renderer.getWidth();
    int h = renderer.getHeight();

    renderer.drawRect({ 0, 0, w, h }, { 255, 255, 255, 1 }, true);

    renderer.drawText(w / 2 - 90, 30, "ROLLERCOASTER", { 20, 20, 50, 255 }, 20);
    renderer.drawText(w / 2 - 90, 55, "REVOLUTION 99", { 255, 200, 50, 255 }, 20);

    const char* labels[] = { "Career Mode", "Survival Mode", "Hot Seat", "Ghost Race", "Options", "Credits" };

    int startY = 100;
    for (int i = 0; i < COUNT; ++i) {
        int y = startY + i * 30;
        bool selected = (i == m_selectedOption);

        if (selected) {
            int boxX = w / 2 - 60;
            int boxY = y - 2;
            int boxW = 120;
            int boxH = 22;

            // Gradient Loop
            for (int j = 0; j < boxW; j++) {
                // Calculate the interpolation factor (0.0 to 1.0) based on the box width
                float t = (float)j / (float)boxW;

                // Linearly interpolate between Pink (253, 3, 251) and Green (142, 210, 20)
                uint8_t r = (uint8_t)(253 + (142 - 253) * t);
                uint8_t g = (uint8_t)(3 + (210 - 3) * t);
                uint8_t b = (uint8_t)(251 + (20 - 251) * t);

                // Draw a 1-pixel wide vertical segment of the background
                renderer.drawRect({ boxX + j, boxY, 1, boxH }, { r, g, b, 255 }, true);
            }

            // Draw the yellow border around the whole box (after the gradient)
            renderer.drawRect({ boxX, boxY, boxW, boxH }, { 255, 255, 0, 255 }, false);
        }

        renderer.drawText(
            w / 2 - 45, y, labels[i], selected ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 200, 200, 200, 255 }, 14);
    }

    renderer.drawText(w / 2 - 70, h - 20, "UP/DOWN: Navigate  SPACE: Select", { 150, 150, 150, 255 }, 10);
}

void MenuState::handleInput(const InputState& input) {
    if (input.menu_up) {
        m_selectedOption = (m_selectedOption - 1 + COUNT) % COUNT;
    }
    if (input.menu_down) {
        m_selectedOption = (m_selectedOption + 1) % COUNT;
    }
    if (input.menu_confirm) {
        switch (m_selectedOption) {
            case Career:
                m_game->pushState(std::make_unique<CareerState>(*m_game));
                break;
            case Survival:
                m_game->pushState(std::make_unique<SurvivalState>(*m_game));
                break;
            case HotSeat:
                m_game->pushState(std::make_unique<HotSeatState>(*m_game));
                break;
            case GhostRace:
                m_game->pushState(std::make_unique<GhostRaceState>(*m_game));
                break;
            case Options:
                break;
            case Credits:
                break;
        }
    }
    if (input.menu_cancel) {
        m_game->quit();
    }
}

} // namespace rr99
