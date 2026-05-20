#ifndef RR99_RENDER_UI_LEVELSELECTSCREEN_H
#define RR99_RENDER_UI_LEVELSELECTSCREEN_H

#include "input/InputState.h"

namespace rr99 {

class Renderer;

class LevelSelectScreen {
public:
    LevelSelectScreen() = default;

    void enter(int unlockedCount, const int* stars, const int* scores);
    void tick(float dt);
    void draw(Renderer& renderer) const;
    void handleInput(const InputState& input);

    int getSelectedLevel() const {
        return m_selectedLevel;
    }
    bool shouldConfirm() const {
        return m_confirm;
    }
    bool shouldCancel() const {
        return m_cancel;
    }
    void resetFlags() {
        m_confirm = false;
        m_cancel = false;
    }

private:
    bool m_active = false;
    int m_selectedLevel = 0;
    int m_unlockedCount = 0;
    int m_stars[99] = { 0 };
    int m_scores[99] = { 0 };
    float m_animTimer = 0.0f;
    bool m_confirm = false;
    bool m_cancel = false;
};

} // namespace rr99

#endif // RR99_RENDER_UI_LEVELSELECTSCREEN_H
