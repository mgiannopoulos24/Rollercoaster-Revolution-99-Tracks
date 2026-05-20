#ifndef RR99_GAME_CAREERSTATE_H
#define RR99_GAME_CAREERSTATE_H

#include "core/GameState.h"

#include <memory>

namespace rr99 {

class Game;
class ProgressManager;
class LevelSelectScreen;

class CareerState : public GameState {
public:
    explicit CareerState(Game& game);
    ~CareerState() override;

    void enter() override;
    void exit() override;
    void tick(float dt) override;
    void draw(Renderer& renderer) override;
    void handleInput(const InputState& input) override;

private:
    void startLevel(int level);
    void handleLevelResult();

    Game* m_game;
    std::unique_ptr<ProgressManager> m_progress;
    std::unique_ptr<LevelSelectScreen> m_levelSelect;
    bool m_handlingResult = false;
    int m_currentPlaying = -1;
};

} // namespace rr99

#endif // RR99_GAME_CAREERSTATE_H
