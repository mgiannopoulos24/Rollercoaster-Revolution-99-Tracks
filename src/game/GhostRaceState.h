#ifndef RR99_GAME_GHOSTRACESTATE_H
#define RR99_GAME_GHOSTRACESTATE_H

#include "core/GameState.h"

#include <memory>
#include <vector>

namespace rr99 {

class Game;
class Renderer;
struct InputState;

class GhostRaceState : public GameState {
public:
    explicit GhostRaceState(Game& game);
    ~GhostRaceState() override = default;

    void enter() override;
    void exit() override;
    void tick(float dt) override;
    void draw(Renderer& renderer) override;
    void handleInput(const InputState& input) override;

private:
    void startRace(int level);
    void handleRaceResult();

    Game* m_game;
    int m_selectedLevel = 0;
    bool m_handlingResult = false;
    bool m_showResult = false;
    int m_lastScore = 0;
};

} // namespace rr99

#endif // RR99_GAME_GHOSTRACESTATE_H
