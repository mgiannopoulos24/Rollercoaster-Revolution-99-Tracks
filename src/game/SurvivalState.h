#ifndef RR99_GAME_SURVIVALSTATE_H
#define RR99_GAME_SURVIVALSTATE_H

#include "core/GameState.h"

#include <memory>

namespace rr99 {

class Game;
class LoadingScreen;
class TrackLayout;
class TrackElements;

class SurvivalState : public GameState {
public:
    explicit SurvivalState(Game& game);
    ~SurvivalState() override;

    void enter() override;
    void exit() override;
    void tick(float dt) override;
    void draw(Renderer& renderer) override;
    void handleInput(const InputState& input) override;

private:
    void startRun();
    void handleRunResult();

    Game* m_game;
    std::unique_ptr<LoadingScreen> m_loadingScreen;
    int m_score = 0;
    int m_runCount = 0;
    int m_difficulty = 1;
    bool m_loading = false;
    bool m_handlingResult = false;
};

} // namespace rr99

#endif // RR99_GAME_SURVIVALSTATE_H
