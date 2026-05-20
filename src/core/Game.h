#ifndef RR99_CORE_GAME_H
#define RR99_CORE_GAME_H

#include <memory>
#include <string>
#include <vector>

namespace rr99 {

class Renderer;
class GameState;
struct InputState;
class InputManager;
class EventBus;

class Game {
public:
    Game();
    ~Game();

    bool initialize(const std::string& title, int width, int height, bool fullscreen);
    void run();
    void quit();

    // State stack management
    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void replaceState(std::unique_ptr<GameState> state);
    GameState* getCurrentState() const;

    // Slow motion
    void setSlowMotion(bool enabled);
    bool isSlowMotion() const {
        return m_slowMotion;
    }

    struct LevelResult {
        bool active = false;
        bool completed = false;
        int levelNumber = 0;
        int score = 0;
        int stars = 0;
    };

    void setLevelResult(bool completed, int levelNumber, int score, int stars) {
        m_levelResult.active = true;
        m_levelResult.completed = completed;
        m_levelResult.levelNumber = levelNumber;
        m_levelResult.score = score;
        m_levelResult.stars = stars;
    }
    const LevelResult& getLevelResult() const {
        return m_levelResult;
    }
    void clearLevelResult() {
        m_levelResult.active = false;
    }

    // Accessors
    Renderer& getRenderer() {
        return *m_renderer;
    }
    InputManager& getInputManager() {
        return *m_inputManager;
    }
    EventBus& getEventBus() {
        return *m_eventBus;
    }
    int getTargetFPS() const {
        return m_slowMotion ? 30 : 60;
    }

private:
    void handleWindowEvent(const InputState& input);
    void handleResize(int newW, int newH);
    void handleFocusChange(bool focused);

    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<InputManager> m_inputManager;
    std::unique_ptr<EventBus> m_eventBus;

    std::vector<std::unique_ptr<GameState>> m_stateStack;

    LevelResult m_levelResult;

    bool m_running = false;
    bool m_paused = false;
    bool m_slowMotion = false;
    bool m_focused = true;

    static constexpr float FIXED_DT = 1.0f / 60.0f;
};

} // namespace rr99

#endif // RR99_CORE_GAME_H
