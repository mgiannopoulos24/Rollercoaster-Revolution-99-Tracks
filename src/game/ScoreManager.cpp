#include "ScoreManager.h"

#include <algorithm>
#include <cmath>

namespace rr99 {

void ScoreManager::reset() {
    m_score = 0;
    m_tokensCollected = 0;
    m_stars = 0;
    m_speedBonus = 0.0f;
    m_completionTime = 0.0f;
}

void ScoreManager::addTokenScore(int points) {
    m_tokensCollected++;
    m_score += points;
}

void ScoreManager::updateSpeedBonus(float happiness, float dt) {
    if (happiness > 0.6f) {
        m_speedBonus += 10.0f * dt;
    }
}

void ScoreManager::setCompletionTime(float time) {
    m_completionTime = time;
}

void ScoreManager::calculateFinalScore(float happinessMultiplier, float passengerMultiplier) {
    float timeBonus = 0.0f;
    if (m_completionTime < m_parTime) {
        timeBonus = (m_parTime - m_completionTime) * 5.0f;
    }

    float rawScore = static_cast<float>(m_score) + m_speedBonus + timeBonus;
    rawScore *= happinessMultiplier * passengerMultiplier;

    m_score = static_cast<int>(std::round(rawScore));

    if (m_score >= 5000)
        m_stars = 3;
    else if (m_score >= 2500)
        m_stars = 2;
    else if (m_score >= 1000)
        m_stars = 1;
    else
        m_stars = 0;
}

} // namespace rr99
