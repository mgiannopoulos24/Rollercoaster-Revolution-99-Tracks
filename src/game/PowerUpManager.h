#ifndef RR99_GAME_POWERUPMANAGER_H
#define RR99_GAME_POWERUPMANAGER_H

#include "track/TrackElements.h"

#include <cstdint>

namespace rr99 {

class Train;
class CoasterPhysics;

class PowerUpManager {
public:
    PowerUpManager() = default;

    void activate(PowerUpType type);
    void update(float dt, Train& train, CoasterPhysics& physics);
    void deactivate();

    bool isActive() const {
        return m_active;
    }
    PowerUpType getActiveType() const {
        return m_activeType;
    }
    float getRemainingTime() const {
        return m_remainingTime;
    }
    float getTotalDuration() const {
        return m_totalDuration;
    }

    bool isNitroActive() const {
        return m_active && m_activeType == PowerUpType::NitroBoost;
    }
    bool isStickyWheelsActive() const {
        return m_active && m_activeType == PowerUpType::StickyWheels;
    }
    bool isWingsActive() const {
        return m_active && m_activeType == PowerUpType::Wings;
    }
    bool isMagnetActive() const {
        return m_active && m_activeType == PowerUpType::Magnet;
    }
    bool isSlowMotionActive() const {
        return m_active && m_activeType == PowerUpType::SlowMotion;
    }

    void reset();

private:
    bool m_active = false;
    PowerUpType m_activeType = PowerUpType::NitroBoost;
    float m_remainingTime = 0.0f;
    float m_totalDuration = 0.0f;
};

} // namespace rr99

#endif // RR99_GAME_POWERUPMANAGER_H
