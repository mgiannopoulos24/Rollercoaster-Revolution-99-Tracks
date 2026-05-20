#include "HappinessSystem.h"

#include <algorithm>
#include <cmath>

namespace rr99 {

void HappinessSystem::update(float speed, float segmentMinSpeed, float segmentMaxSpeed) {
    float sweetSpot = (segmentMinSpeed + segmentMaxSpeed) * 0.5f;
    float tolerance = (segmentMaxSpeed - segmentMinSpeed) * 0.3f;

    if (speed < segmentMinSpeed - tolerance) {
        m_happiness = std::max(0.0f, m_happiness - 0.02f);
    } else if (speed > segmentMaxSpeed + tolerance) {
        m_happiness = std::max(0.0f, m_happiness - 0.03f);
    } else {
        float distFromSweet = std::abs(speed - sweetSpot);
        if (distFromSweet < tolerance * 0.5f) {
            m_happiness = std::min(1.0f, m_happiness + 0.01f);
        } else {
            m_happiness = std::min(1.0f, m_happiness + 0.005f);
        }
    }

    if (m_happiness < 0.2f) {
        m_level = HappinessLevel::Miserable;
    } else if (m_happiness < 0.4f) {
        m_level = HappinessLevel::Bored;
    } else if (m_happiness < 0.6f) {
        m_level = HappinessLevel::Neutral;
    } else if (m_happiness < 0.8f) {
        m_level = HappinessLevel::Happy;
    } else {
        m_level = HappinessLevel::Thrilled;
    }
}

void HappinessSystem::reset() {
    m_happiness = 0.5f;
    m_level = HappinessLevel::Neutral;
}

float HappinessSystem::getScoreMultiplier(float happiness) {
    if (happiness < 0.2f)
        return 0.5f;
    if (happiness < 0.4f)
        return 0.75f;
    if (happiness < 0.6f)
        return 1.0f;
    if (happiness < 0.8f)
        return 1.25f;
    return 1.5f;
}

const char* HappinessSystem::getLevelName(HappinessLevel level) {
    switch (level) {
        case HappinessLevel::Miserable:
            return "Miserable";
        case HappinessLevel::Bored:
            return "Bored";
        case HappinessLevel::Neutral:
            return "Neutral";
        case HappinessLevel::Happy:
            return "Happy";
        case HappinessLevel::Thrilled:
            return "Thrilled";
    }
    return "Unknown";
}

} // namespace rr99
