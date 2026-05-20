#ifndef RR99_GAME_TRAINSELECTOR_H
#define RR99_GAME_TRAINSELECTOR_H

#include "physics/Train.h"

#include <cstdint>

namespace rr99 {

class Renderer;
struct InputState;

class TrainSelector {
public:
    TrainSelector() = default;

    void enter(int levelNumber, bool hasRequired, TrainType required);
    void exit();
    void tick(float dt);
    void draw(Renderer& renderer);
    void handleInput(const InputState& input);

    TrainType getSelectedType() const {
        return m_selectedType;
    }
    bool isConfirmed() const {
        return m_confirmed;
    }
    bool isActive() const {
        return m_active;
    }
    void resetConfirmed() {
        m_confirmed = false;
    }

private:
    TrainType m_selectedType = TrainType::Medium;
    int m_levelNumber = 0;
    bool m_hasRequired = false;
    TrainType m_requiredType = TrainType::Medium;
    bool m_confirmed = false;
    bool m_active = false;
    float m_animTimer = 0.0f;
};

} // namespace rr99

#endif // RR99_GAME_TRAINSELECTOR_H
