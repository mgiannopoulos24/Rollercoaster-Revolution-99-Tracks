#include "PowerUpManager.h"

#include "physics/CoasterPhysics.h"
#include "physics/Train.h"

namespace rr99 {

void PowerUpManager::activate(PowerUpType type) {
    m_active = true;
    m_activeType = type;
    m_totalDuration = TrackElements::getPowerUpDuration(type);
    m_remainingTime = m_totalDuration;
}

void PowerUpManager::update(float dt, Train& train, CoasterPhysics& physics) {
    if (!m_active)
        return;

    m_remainingTime -= dt;

    if (m_remainingTime <= 0.0f) {
        deactivate();
        return;
    }

    switch (m_activeType) {
        case PowerUpType::NitroBoost:
            physics.applyNitroBoost(train);
            break;
        case PowerUpType::SlowMotion:
            break;
        default:
            break;
    }
}

void PowerUpManager::deactivate() {
    m_active = false;
    m_remainingTime = 0.0f;
}

void PowerUpManager::reset() {
    m_active = false;
    m_remainingTime = 0.0f;
}

} // namespace rr99
