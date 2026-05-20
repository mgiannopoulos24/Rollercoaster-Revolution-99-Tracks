#ifndef RR99_GAME_PROGRESSMANAGER_H
#define RR99_GAME_PROGRESSMANAGER_H

#include <string>

namespace rr99 {

class ProgressManager {
public:
    ProgressManager() = default;

    void load();
    void save();

    bool isLevelUnlocked(int level) const;
    int getLevelScore(int level) const;
    int getLevelStars(int level) const;
    int getLevelTokens(int level) const;
    int getUnlockedLevels() const {
        return m_unlockedCount;
    }

    void unlockLevel(int level);
    void setLevelScore(int level, int score, int stars, int tokens);

private:
    static constexpr int TOTAL_LEVELS = 99;
    bool m_unlocked[TOTAL_LEVELS] = { false };
    int m_scores[TOTAL_LEVELS] = { 0 };
    int m_stars[TOTAL_LEVELS] = { 0 };
    int m_tokens[TOTAL_LEVELS] = { 0 };
    int m_unlockedCount = 1;
};

} // namespace rr99

#endif // RR99_GAME_PROGRESSMANAGER_H
