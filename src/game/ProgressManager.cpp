#include "ProgressManager.h"

#include "db/DatabaseManager.h"

#include <algorithm>
#include <cstring>

namespace rr99 {

void ProgressManager::load() {
    m_unlocked[0] = true;
    m_unlockedCount = 1;

    DatabaseManager::instance().queryExec(
        "SELECT level_number, best_score, stars, tokens_collected FROM levels ORDER BY level_number",
        [this](sqlite3_stmt* stmt) {
            int level = sqlite3_column_int(stmt, 0) - 1;
            if (level >= 0 && level < TOTAL_LEVELS) {
                m_scores[level] = sqlite3_column_int(stmt, 1);
                m_stars[level] = sqlite3_column_int(stmt, 2);
                m_tokens[level] = sqlite3_column_int(stmt, 3);
                m_unlocked[level] = true;
            }
        });

    // Determine unlocked levels based on completion
    int maxCompleted = 0;
    for (int i = 0; i < TOTAL_LEVELS; ++i) {
        if (m_stars[i] > 0) {
            maxCompleted = std::max(maxCompleted, i + 1);
        }
    }
    m_unlockedCount = std::min(maxCompleted + 1, TOTAL_LEVELS);
    for (int i = 0; i < m_unlockedCount; ++i) {
        m_unlocked[i] = true;
    }
}

void ProgressManager::save() {
    // Save individual levels - handled by setLevelScore which does immediate writes
}

bool ProgressManager::isLevelUnlocked(int level) const {
    if (level < 0 || level >= TOTAL_LEVELS)
        return false;
    return m_unlocked[level];
}

int ProgressManager::getLevelScore(int level) const {
    if (level < 0 || level >= TOTAL_LEVELS)
        return 0;
    return m_scores[level];
}

int ProgressManager::getLevelStars(int level) const {
    if (level < 0 || level >= TOTAL_LEVELS)
        return 0;
    return m_stars[level];
}

int ProgressManager::getLevelTokens(int level) const {
    if (level < 0 || level >= TOTAL_LEVELS)
        return 0;
    return m_tokens[level];
}

void ProgressManager::unlockLevel(int level) {
    if (level < 0 || level >= TOTAL_LEVELS)
        return;
    m_unlocked[level] = true;
    m_unlockedCount = std::max(m_unlockedCount, level + 1);
}

void ProgressManager::setLevelScore(int level, int score, int stars, int tokens) {
    if (level < 0 || level >= TOTAL_LEVELS)
        return;

    if (score > m_scores[level]) {
        m_scores[level] = score;
    }
    if (stars > m_stars[level]) {
        m_stars[level] = stars;
    }
    if (tokens > m_tokens[level]) {
        m_tokens[level] = tokens;
    }

    DatabaseManager::instance().execute(
        "INSERT INTO levels (level_number, unlocked, best_score, stars, tokens_collected) VALUES (" +
        std::to_string(level + 1) + ", 1, " + std::to_string(m_scores[level]) + ", " + std::to_string(m_stars[level]) + ", " +
        std::to_string(m_tokens[level]) +
        ") ON CONFLICT(level_number) DO UPDATE SET unlocked=1, best_score=" + std::to_string(m_scores[level]) +
        ", stars=" + std::to_string(m_stars[level]) + ", tokens_collected=" + std::to_string(m_tokens[level]));
}

} // namespace rr99
