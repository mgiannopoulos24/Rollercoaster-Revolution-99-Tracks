#ifndef RR99_RENDER_UI_HUD_H
#define RR99_RENDER_UI_HUD_H

#include <string>

namespace rr99 {

class Renderer;

class HUD {
public:
    HUD() = default;

    void setSpeed(float speed) {
        m_speed = speed;
    }
    void setTokens(int count, int total) {
        m_tokens = count;
        m_totalTokens = total;
    }
    void setHappiness(float happiness) {
        m_happiness = happiness;
    }
    void setPassengers(int remaining, int total) {
        m_passengers = remaining;
        m_totalPassengers = total;
    }
    void setPowerUp(const std::string& name, float remaining, float duration);
    void clearPowerUp();
    void setTime(float time) {
        m_time = time;
    }
    void setScore(int score) {
        m_score = score;
    }
    void setLevel(int level) {
        m_level = level;
    }

    void draw(Renderer& renderer) const;

private:
    float m_speed = 0.0f;
    int m_tokens = 0;
    int m_totalTokens = 0;
    float m_happiness = 0.5f;
    int m_passengers = 0;
    int m_totalPassengers = 0;
    bool m_hasPowerUp = false;
    std::string m_powerUpName;
    float m_powerUpRemaining = 0.0f;
    float m_powerUpDuration = 0.0f;
    float m_time = 0.0f;
    int m_score = 0;
    int m_level = 0;
};

} // namespace rr99

#endif // RR99_RENDER_UI_HUD_H
