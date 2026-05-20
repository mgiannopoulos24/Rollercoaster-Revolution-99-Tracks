#ifndef RR99_GAME_HAPPINESSSYSTEM_H
#define RR99_GAME_HAPPINESSSYSTEM_H

#include <cstdint>

namespace rr99 {

enum class HappinessLevel : uint8_t { Miserable, Bored, Neutral, Happy, Thrilled };

class HappinessSystem {
public:
    HappinessSystem() = default;

    void update(float speed, float segmentMinSpeed, float segmentMaxSpeed);

    float getHappiness() const {
        return m_happiness;
    }
    HappinessLevel getLevel() const {
        return m_level;
    }
    void reset();

    static float getScoreMultiplier(float happiness);
    static const char* getLevelName(HappinessLevel level);

private:
    float m_happiness = 0.5f;
    HappinessLevel m_level = HappinessLevel::Neutral;
};

} // namespace rr99

#endif // RR99_GAME_HAPPINESSSYSTEM_H
