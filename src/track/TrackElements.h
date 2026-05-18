#ifndef RR99_TRACK_TRACKELEMENTS_H
#define RR99_TRACK_TRACKELEMENTS_H

#include <cstdint>
#include <memory>
#include <vector>

namespace rr99 {

enum class TokenState : uint8_t { Available, Collected, MagnetPulled };

struct SmileToken {
    int segmentIndex = 0;
    float position = 0.0f;
    float speedMin = 0.0f;
    float speedMax = 100.0f;
    bool collected = false;
    TokenState state = TokenState::Available;
};

enum class PowerUpType : uint8_t { NitroBoost, StickyWheels, Wings, Magnet, SlowMotion, COUNT };

enum class PowerUpState : uint8_t { Available, Active, Expired };

struct PowerUp {
    PowerUpType type = PowerUpType::NitroBoost;
    int segmentIndex = 0;
    float position = 0.0f;
    bool collected = false;
    PowerUpState state = PowerUpState::Available;
    float activeDuration = 5.0f;
    float activeTimer = 0.0f;
};

enum class HazardType : uint8_t { FallingTrack, None };

struct Hazard {
    HazardType type = HazardType::None;
    int segmentIndex = 0;
    float position = 0.0f;
    bool triggered = false;
};

struct ElementPosition {
    int segmentIndex = 0;
    float offset = 0.0f;
};

class TrackElements {
public:
    TrackElements() = default;

    void addToken(const SmileToken& token);
    void addPowerUp(const PowerUp& powerUp);
    void addHazard(const Hazard& hazard);

    SmileToken* getToken(int index);
    const SmileToken* getToken(int index) const;
    PowerUp* getPowerUp(int index);
    const PowerUp* getPowerUp(int index) const;
    Hazard* getHazard(int index);
    const Hazard* getHazard(int index) const;

    int getTokenCount() const {
        return static_cast<int>(m_tokens.size());
    }
    int getPowerUpCount() const {
        return static_cast<int>(m_powerUps.size());
    }
    int getHazardCount() const {
        return static_cast<int>(m_hazards.size());
    }

    bool collectToken(int index, float currentSpeed);
    bool collectPowerUp(int index);

    void clear();

    static const char* getPowerUpName(PowerUpType type);
    static float getPowerUpDuration(PowerUpType type);

private:
    std::vector<SmileToken> m_tokens;
    std::vector<PowerUp> m_powerUps;
    std::vector<Hazard> m_hazards;
};

} // namespace rr99

#endif // RR99_TRACK_TRACKELEMENTS_H
