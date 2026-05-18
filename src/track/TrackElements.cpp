#include "TrackElements.h"

namespace rr99 {

void TrackElements::addToken(const SmileToken& token) {
    m_tokens.push_back(token);
}

void TrackElements::addPowerUp(const PowerUp& powerUp) {
    m_powerUps.push_back(powerUp);
}

void TrackElements::addHazard(const Hazard& hazard) {
    m_hazards.push_back(hazard);
}

SmileToken* TrackElements::getToken(int index) {
    if (index < 0 || index >= static_cast<int>(m_tokens.size()))
        return nullptr;
    return &m_tokens[index];
}

const SmileToken* TrackElements::getToken(int index) const {
    if (index < 0 || index >= static_cast<int>(m_tokens.size()))
        return nullptr;
    return &m_tokens[index];
}

PowerUp* TrackElements::getPowerUp(int index) {
    if (index < 0 || index >= static_cast<int>(m_powerUps.size()))
        return nullptr;
    return &m_powerUps[index];
}

const PowerUp* TrackElements::getPowerUp(int index) const {
    if (index < 0 || index >= static_cast<int>(m_powerUps.size()))
        return nullptr;
    return &m_powerUps[index];
}

Hazard* TrackElements::getHazard(int index) {
    if (index < 0 || index >= static_cast<int>(m_hazards.size()))
        return nullptr;
    return &m_hazards[index];
}

const Hazard* TrackElements::getHazard(int index) const {
    if (index < 0 || index >= static_cast<int>(m_hazards.size()))
        return nullptr;
    return &m_hazards[index];
}

bool TrackElements::collectToken(int index, float currentSpeed) {
    if (index < 0 || index >= static_cast<int>(m_tokens.size()))
        return false;
    auto& token = m_tokens[index];
    if (token.collected)
        return false;
    if (currentSpeed >= token.speedMin && currentSpeed <= token.speedMax) {
        token.collected = true;
        token.state = TokenState::Collected;
        return true;
    }
    return false;
}

bool TrackElements::collectPowerUp(int index) {
    if (index < 0 || index >= static_cast<int>(m_powerUps.size()))
        return false;
    auto& pu = m_powerUps[index];
    if (pu.collected)
        return false;
    pu.collected = true;
    pu.state = PowerUpState::Active;
    return true;
}

void TrackElements::clear() {
    m_tokens.clear();
    m_powerUps.clear();
    m_hazards.clear();
}

const char* TrackElements::getPowerUpName(PowerUpType type) {
    switch (type) {
        case PowerUpType::NitroBoost:
            return "nitro_boost";
        case PowerUpType::StickyWheels:
            return "sticky_wheels";
        case PowerUpType::Wings:
            return "wings";
        case PowerUpType::Magnet:
            return "magnet";
        case PowerUpType::SlowMotion:
            return "slow_motion";
        case PowerUpType::COUNT:
            return "unknown";
    }
    return "unknown";
}

float TrackElements::getPowerUpDuration(PowerUpType type) {
    switch (type) {
        case PowerUpType::NitroBoost:
            return 3.0f;
        case PowerUpType::StickyWheels:
            return 5.0f;
        case PowerUpType::Wings:
            return 4.0f;
        case PowerUpType::Magnet:
            return 6.0f;
        case PowerUpType::SlowMotion:
            return 4.0f;
        case PowerUpType::COUNT:
            return 0.0f;
    }
    return 0.0f;
}

} // namespace rr99
