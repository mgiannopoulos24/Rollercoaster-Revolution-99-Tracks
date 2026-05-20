#include "TrainSelector.h"

#include "input/InputState.h"
#include "render/Renderer.h"

#include <string>

namespace rr99 {

void TrainSelector::enter(int levelNumber, bool hasRequired, TrainType required) {
    m_levelNumber = levelNumber;
    m_hasRequired = hasRequired;
    m_requiredType = required;
    m_selectedType = hasRequired ? required : TrainType::Medium;
    m_confirmed = false;
    m_active = true;
    m_animTimer = 0.0f;
}

void TrainSelector::exit() {
    m_active = false;
}

void TrainSelector::tick(float dt) {
    if (!m_active)
        return;
    m_animTimer += dt;
}

void TrainSelector::draw(Renderer& renderer) {
    if (!m_active)
        return;

    int w = renderer.getWidth();
    int h = renderer.getHeight();

    SDL_Rect bg = { 0, 0, w, h };
    renderer.drawRect(&bg, { 20, 20, 40, 255 }, true);

    std::string title = "SELECT TRAIN - Level " + std::to_string(m_levelNumber);
    renderer.drawText(w / 2 - 70, 15, title, { 255, 255, 255, 255 }, 16);

    if (m_hasRequired) {
        renderer.drawText(w / 2 - 80, 38, "Required train for this level!", { 255, 200, 0, 255 }, 12);
    }

    const char* names[] = { "Light", "Medium", "Heavy" };
    const char* descs[] = { "Fast acceleration, low grip", "Balanced stats", "Slow acceleration, high grip" };

    int startY = 60;
    for (int i = 0; i < 3; ++i) {
        TrainType tt = static_cast<TrainType>(i);
        auto props = Train::getDefaultProperties(tt);

        int y = startY + i * 70;
        bool selected = (m_selectedType == tt);

        SDL_Color border = selected ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 100, 100, 100, 255 };
        SDL_Rect box = { 15, y, w - 30, 62 };
        renderer.drawRect(&box, border, false);

        SDL_Color nameColor = selected ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 200, 200, 200, 255 };
        renderer.drawText(25, y + 4, names[i], nameColor, 14);

        std::string massStr = "Mass:" + std::to_string(static_cast<int>(props.mass));
        std::string gripStr = "Grip:" + std::to_string(static_cast<int>(props.grip * 100.0f));
        std::string accelStr = "Accel:" + std::to_string(static_cast<int>(props.acceleration));
        std::string speedStr = "Speed:" + std::to_string(static_cast<int>(props.maxSpeed));

        std::string statLine = massStr + "%  " + gripStr + "%  " + accelStr + "  " + speedStr;
        renderer.drawText(25, y + 22, statLine, { 180, 180, 180, 255 }, 10);
        renderer.drawText(25, y + 38, descs[i], { 150, 150, 150, 255 }, 10);
    }

    renderer.drawText(w / 2 - 80, h - 25, "UP/DOWN: Select   SPACE: Confirm", { 200, 200, 200, 255 }, 10);
}

void TrainSelector::handleInput(const InputState& input) {
    if (!m_active)
        return;

    if (input.menu_up) {
        int current = static_cast<int>(m_selectedType);
        current = (current - 1 + 3) % 3;
        m_selectedType = static_cast<TrainType>(current);
    }
    if (input.menu_down) {
        int current = static_cast<int>(m_selectedType);
        current = (current + 1) % 3;
        m_selectedType = static_cast<TrainType>(current);
    }
    if (input.menu_confirm) {
        m_confirmed = true;
    }
}

} // namespace rr99
