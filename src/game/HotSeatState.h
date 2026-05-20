#ifndef RR99_GAME_HOTSEATSTATE_H
#define RR99_GAME_HOTSEATSTATE_H

#include "core/GameState.h"

#include <memory>

namespace rr99 {

class Game;

class HotSeatState : public GameState {
public:
    explicit HotSeatState(Game& game);
    ~HotSeatState() override = default;

    void enter() override;
    void exit() override;
    void tick(float dt) override;
    void draw(Renderer& renderer) override;
    void handleInput(const InputState& input) override;

private:
    void startPlayerTurn();
    void handleTurnResult();

    Game* m_game;
    int m_currentPlayer = 0;
    int m_scores[2] = { 0, 0 };
    bool m_playing = false;
    bool m_handlingResult = false;
    bool m_gameOver = false;
};

} // namespace rr99

#endif // RR99_GAME_HOTSEATSTATE_H
