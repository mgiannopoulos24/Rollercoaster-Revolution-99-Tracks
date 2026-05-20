#include "LevelSelectScreen.h"

#include "render/Renderer.h"

#include <algorithm>
#include <string>

namespace rr99 {

void LevelSelectScreen::enter(int unlockedCount, const int* stars, const int* scores) {
    m_active = true;
    m_unlockedCount = unlockedCount;
    m_selectedLevel = 0;
    m_animTimer = 0.0f;
    m_confirm = false;
    m_cancel = false;
    for (int i = 0; i < 99; ++i) {
        m_stars[i] = stars[i];
        m_scores[i] = scores[i];
    }
}

void LevelSelectScreen::tick(float dt) {
    if (!m_active)
        return;
    m_animTimer += dt;
}

void LevelSelectScreen::handleInput(const InputState& input) {
    if (!m_active)
        return;

    const int cols = 5;

    if (input.menu_up) {
        m_selectedLevel = std::max(0, m_selectedLevel - cols);
    }
    if (input.menu_down) {
        m_selectedLevel = std::min(98, m_selectedLevel + cols);
    }
    if (input.menu_confirm) {
        if (m_selectedLevel < m_unlockedCount) {
            m_confirm = true;
        }
    }
    if (input.menu_cancel) {
        m_cancel = true;
    }
}

void LevelSelectScreen::draw(Renderer& renderer) const {
    if (!m_active)
        return;

    int w = renderer.getWidth();
    int h = renderer.getHeight();

    renderer.drawRect({ 0, 0, w, h }, { 20, 20, 50, 255 }, true);
    renderer.drawText(w / 2 - 50, 10, "SELECT LEVEL", { 255, 255, 255, 255 }, 16);

    const int cols = 5;
    const int rows = 5;
    const int levelsPerPage = cols * rows;
    int startLevel = (m_selectedLevel / levelsPerPage) * levelsPerPage;

    int tileW = (w - 40) / cols;
    int tileH = 36;
    int startX = 15;
    int startY = 36;

    for (int i = 0; i < levelsPerPage; ++i) {
        int levelIdx = startLevel + i;
        if (levelIdx >= 99)
            break;

        int col = i % cols;
        int row = i / cols;
        int x = startX + col * (tileW + 4);
        int y = startY + row * (tileH + 4);

        bool unlocked = levelIdx < m_unlockedCount;
        bool selected = (levelIdx == m_selectedLevel);

        SDL_Color bg;
        if (selected && unlocked)
            bg = { 60, 100, 60, 255 };
        else if (unlocked)
            bg = { 40, 60, 40, 255 };
        else
            bg = { 25, 25, 25, 255 };

        renderer.drawRect({ x, y, tileW, tileH }, bg, true);

        if (selected && unlocked) {
            renderer.drawRect({ x, y, tileW, tileH }, { 255, 255, 0, 255 }, false);
        } else if (unlocked) {
            renderer.drawRect({ x, y, tileW, tileH }, { 80, 80, 80, 255 }, false);
        }

        std::string label = std::to_string(levelIdx + 1);
        SDL_Color textColor = unlocked ? SDL_Color{ 255, 255, 255, 255 } : SDL_Color{ 60, 60, 60, 255 };
        renderer.drawText(x + tileW / 2 - 8, y + 3, label, textColor, 12);

        if (unlocked && m_stars[levelIdx] > 0) {
            std::string starsStr = std::string(m_stars[levelIdx], '*');
            renderer.drawText(x + tileW / 2 - 8, y + 18, starsStr, { 255, 255, 0, 255 }, 10);
        }
    }

    int totalPages = (99 + levelsPerPage - 1) / levelsPerPage;
    int currentPage = m_selectedLevel / levelsPerPage;
    std::string pageStr = "Page " + std::to_string(currentPage + 1) + "/" + std::to_string(totalPages);
    renderer.drawText(w / 2 - 30, h - 25, pageStr, { 200, 200, 200, 255 }, 10);

    renderer.drawText(w / 2 - 100, h - 12, "ARROWS: Navigate  SPACE: Select  ESC: Back", { 150, 150, 150, 255 }, 10);
}

} // namespace rr99
