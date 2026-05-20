#include "HUD.h"

#include "render/Renderer.h"

#include <algorithm>
#include <string>

namespace rr99 {

void HUD::setPowerUp(const std::string& name, float remaining, float duration) {
    m_hasPowerUp = true;
    m_powerUpName = name;
    m_powerUpRemaining = remaining;
    m_powerUpDuration = duration;
}

void HUD::clearPowerUp() {
    m_hasPowerUp = false;
}

void HUD::draw(Renderer& renderer) const {
    int w = renderer.getWidth();
    int h = renderer.getHeight();

    std::string speedText = "SPD: " + std::to_string(static_cast<int>(m_speed));
    renderer.drawText(5, h - 20, speedText, { 255, 255, 255, 255 }, 12);

    std::string tokenText = "TKNS: " + std::to_string(m_tokens) + "/" + std::to_string(m_totalTokens);
    renderer.drawText(5, 5, tokenText, { 255, 255, 0, 255 }, 12);

    std::string happyText = "HAP: " + std::to_string(static_cast<int>(m_happiness * 100.0f)) + "%";
    renderer.drawText(w / 2 - 25, 5, happyText, { 0, 255, 0, 255 }, 12);

    std::string passengerText = "PASS: " + std::to_string(m_passengers) + "/" + std::to_string(m_totalPassengers);
    renderer.drawText(w - 80, 5, passengerText, { 255, 255, 255, 255 }, 12);

    if (m_hasPowerUp && m_powerUpDuration > 0.0f) {
        float ratio = std::max(0.0f, m_powerUpRemaining / m_powerUpDuration);
        std::string puText = m_powerUpName + " [" + std::to_string(static_cast<int>(ratio * 100.0f)) + "%]";
        renderer.drawText(w / 2 - 40, h - 40, puText, { 0, 200, 255, 255 }, 12);
    }

    std::string timeText = "TIME: " + std::to_string(static_cast<int>(m_time)) + "s";
    renderer.drawText(w - 80, h - 20, timeText, { 255, 255, 255, 255 }, 12);

    std::string scoreText = "SCR: " + std::to_string(m_score);
    renderer.drawText(5, 20, scoreText, { 255, 255, 255, 255 }, 10);
}

} // namespace rr99
