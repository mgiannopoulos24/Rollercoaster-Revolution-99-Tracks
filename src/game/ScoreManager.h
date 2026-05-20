#ifndef RR99_GAME_SCOREMANAGER_H
#define RR99_GAME_SCOREMANAGER_H

namespace rr99 {

class ScoreManager {
public:
    ScoreManager() = default;

    void reset();
    void addTokenScore(int points = 100);
    void updateSpeedBonus(float happiness, float dt);
    void setCompletionTime(float time);
    void setParTime(float time) {
        m_parTime = time;
    }
    void calculateFinalScore(float happinessMultiplier, float passengerMultiplier);

    int getScore() const {
        return m_score;
    }
    int getTokensCollected() const {
        return m_tokensCollected;
    }
    int getStarRating() const {
        return m_stars;
    }
    float getCompletionTime() const {
        return m_completionTime;
    }

private:
    int m_score = 0;
    int m_tokensCollected = 0;
    int m_stars = 0;
    float m_speedBonus = 0.0f;
    float m_completionTime = 0.0f;
    float m_parTime = 60.0f;
};

} // namespace rr99

#endif // RR99_GAME_SCOREMANAGER_H
