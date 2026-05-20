#ifndef RR99_RENDER_UI_LOADINGSCREEN_H
#define RR99_RENDER_UI_LOADINGSCREEN_H

#include <string>

namespace rr99 {

class Renderer;

class LoadingScreen {
public:
    LoadingScreen() = default;

    void enter(const std::string& message, int levelNumber = 0);
    void tick(float dt);
    void draw(Renderer& renderer) const;
    bool isDone() const {
        return m_done;
    }
    float getProgress() const {
        return m_progress;
    }

private:
    std::string m_message;
    int m_levelNumber = 0;
    float m_progress = 0.0f;
    float m_timer = 0.0f;
    bool m_done = false;
    bool m_active = false;
};

} // namespace rr99

#endif // RR99_RENDER_UI_LOADINGSCREEN_H
