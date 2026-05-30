#ifndef RR99_GAME_MENUSTATE_H
#define RR99_GAME_MENUSTATE_H

#include "core/GameState.h"

struct SDL_Texture;

namespace rr99 {

class Game;

class MenuState : public GameState {
public:
    explicit MenuState(Game& game);
    ~MenuState() override = default;

    void enter() override;
    void exit() override;
    void tick(float dt) override;
    void draw(Renderer& renderer) override;
    void handleInput(const InputState& input) override;

private:
    enum MenuOption { Career, Survival, HotSeat, GhostRace, Options, Credits, COUNT };

    Game* m_game;
    int m_selectedOption = 0;
    float m_animTimer = 0.0f;
    SDL_Texture* m_bgTexture = nullptr;
};

} // namespace rr99

#endif // RR99_GAME_MENUSTATE_H
