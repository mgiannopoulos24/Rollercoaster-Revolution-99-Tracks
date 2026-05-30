#ifndef RR99_CORE_GAMESTATE_H
#define RR99_CORE_GAMESTATE_H

namespace rr99 {

class Renderer;
struct InputState;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void enter() {
    }
    virtual void exit() {
    }
    virtual void tick(float dt) = 0;
    virtual void draw(Renderer& renderer) = 0;
    virtual void handleInput(const InputState&) {
    }
};

} // namespace rr99

#endif // RR99_CORE_GAMESTATE_H
