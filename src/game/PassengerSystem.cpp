#include "PassengerSystem.h"

#include "physics/Train.h"

#include <algorithm>

namespace rr99 {

void PassengerSystem::initialize(Train& train) {
    m_totalPassengers = train.getTotalPassengers();
    m_remainingPassengers = train.getRemainingPassengers();
    m_ejectedThisFrame = false;
}

void PassengerSystem::update(Train& train, float gForce) {
    m_ejectedThisFrame = false;

    if (gForce > 4.0f) {
        for (int i = 0; i < train.getCarCount(); ++i) {
            if (m_remainingPassengers <= 0)
                break;
            if (gForce > 5.0f + static_cast<float>(i) * 1.5f) {
                train.ejectPassenger(i);
                m_remainingPassengers = train.getRemainingPassengers();
                m_ejectedThisFrame = true;
            }
        }
    }
}

float PassengerSystem::getScoreMultiplier() const {
    if (m_totalPassengers <= 0)
        return 1.0f;
    float ratio = static_cast<float>(m_remainingPassengers) / static_cast<float>(m_totalPassengers);
    return 0.5f + ratio * 1.5f;
}

void PassengerSystem::reset() {
    m_totalPassengers = 0;
    m_remainingPassengers = 0;
    m_ejectedThisFrame = false;
}

} // namespace rr99
