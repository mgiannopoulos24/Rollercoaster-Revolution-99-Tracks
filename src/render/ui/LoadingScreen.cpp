#include "LoadingScreen.h"

#include "render/Renderer.h"

#include <string>

namespace rr99 {

void LoadingScreen::enter(const std::string& message, int levelNumber) {
    m_message = message;
    m_levelNumber = levelNumber;
    m_progress = 0.0f;
    m_timer = 0.0f;
    m_done = false;
    m_active = true;
}

void LoadingScreen::tick(float dt) {
    if (!m_active || m_done)
        return;

    m_timer += dt;

    // Simulate loading progress over ~1.5 seconds
    m_progress = std::min(1.0f, m_timer / 1.5f);

    if (m_progress >= 1.0f) {
        m_done = true;
    }
}

void LoadingScreen::draw(Renderer& renderer) const {
    if (!m_active)
        return;

    int w = renderer.getWidth();
    int h = renderer.getHeight();

    renderer.drawRect({ 0, 0, w, h }, { 10, 10, 30, 255 }, true);

    if (m_levelNumber > 0) {
        std::string levelStr = "Level " + std::to_string(m_levelNumber);
        renderer.drawText(w / 2 - 30, h / 2 - 40, levelStr, { 200, 200, 200, 255 }, 14);
    }

    renderer.drawText(w / 2 - 50, h / 2 - 15, m_message, { 255, 255, 255, 255 }, 12);

    // Progress bar background
    int barW = 150;
    int barH = 12;
    int barX = (w - barW) / 2;
    int barY = h / 2 + 10;
    renderer.drawRect({ barX, barY, barW, barH }, { 40, 40, 40, 255 }, true);
    renderer.drawRect({ barX, barY, barW, barH }, { 100, 100, 100, 255 }, false);

    // Progress bar fill
    int fillW = static_cast<int>(static_cast<float>(barW) * m_progress);
    if (fillW > 0) {
        renderer.drawRect({ barX, barY, fillW, barH }, { 0, 150, 255, 255 }, true);
    }

    std::string pctStr = std::to_string(static_cast<int>(m_progress * 100.0f)) + "%";
    renderer.drawText(w / 2 - 12, barY + barH + 5, pctStr, { 200, 200, 200, 255 }, 10);
}

} // namespace rr99
