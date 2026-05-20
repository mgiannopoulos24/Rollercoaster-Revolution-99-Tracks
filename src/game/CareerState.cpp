#include "CareerState.h"

#include "PlayState.h"
#include "ProgressManager.h"
#include "core/Game.h"
#include "input/InputState.h"
#include "physics/TrackSegment.h"
#include "render/Renderer.h"
#include "render/ui/LevelSelectScreen.h"
#include "track/TrackBuilder.h"
#include "track/TrackElements.h"
#include "track/TrackLayout.h"

#include <string>

namespace rr99 {

CareerState::CareerState(Game& game)
    : m_game(&game)
    , m_progress(std::make_unique<ProgressManager>())
    , m_levelSelect(std::make_unique<LevelSelectScreen>()) {
}

CareerState::~CareerState() = default;

void CareerState::enter() {
    m_progress->load();
    m_handlingResult = false;
    m_currentPlaying = -1;

    int stars[99] = { 0 };
    int scores[99] = { 0 };
    for (int i = 0; i < 99; ++i) {
        stars[i] = m_progress->getLevelStars(i);
        scores[i] = m_progress->getLevelScore(i);
    }
    m_levelSelect->enter(m_progress->getUnlockedLevels(), stars, scores);
}

void CareerState::exit() {
}

void CareerState::tick(float dt) {
    if (m_handlingResult) {
        handleLevelResult();
        return;
    }
    m_levelSelect->tick(dt);
}

void CareerState::draw(Renderer& renderer) {
    m_levelSelect->draw(renderer);
}

void CareerState::handleInput(const InputState& input) {
    if (m_handlingResult)
        return;

    m_levelSelect->handleInput(input);
    if (m_levelSelect->shouldConfirm()) {
        m_levelSelect->resetFlags();
        startLevel(m_levelSelect->getSelectedLevel());
    }
    if (m_levelSelect->shouldCancel()) {
        m_levelSelect->resetFlags();
        m_game->popState();
    }
}

void CareerState::startLevel(int level) {
    m_currentPlaying = level;

    auto track = TrackBuilder().generateProcedural(level, 15);
    auto elements = std::make_unique<TrackElements>();
    TrackBuilder().generateProceduralElements(*elements, *track, level);

    auto playState = std::make_unique<PlayState>(*m_game);
    playState->setLevelData(std::move(track), std::move(elements));
    playState->setTrainType(TrainType::Medium);
    playState->setLevelNumber(level + 1);
    m_game->pushState(std::move(playState));
    m_handlingResult = true;
}

void CareerState::handleLevelResult() {
    auto& result = m_game->getLevelResult();
    if (!result.active)
        return;

    int level = result.levelNumber - 1;
    if (level >= 0 && level < 99) {
        if (result.completed) {
            m_progress->setLevelScore(level, result.score, result.stars, 0);
            if (level + 1 < 99) {
                m_progress->unlockLevel(level + 1);
            }
        }
    }

    m_game->clearLevelResult();
    m_handlingResult = false;
    m_currentPlaying = -1;
    enter();
}

} // namespace rr99
