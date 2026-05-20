#include "SurvivalState.h"

#include "PlayState.h"
#include "core/Game.h"
#include "input/InputState.h"
#include "physics/TrackSegment.h"
#include "render/Renderer.h"
#include "render/ui/LoadingScreen.h"
#include "track/TrackBuilder.h"
#include "track/TrackElements.h"
#include "track/TrackLayout.h"

#include <string>

namespace rr99 {

SurvivalState::SurvivalState(Game& game)
    : m_game(&game)
    , m_loadingScreen(std::make_unique<LoadingScreen>()) {
}

SurvivalState::~SurvivalState() = default;

void SurvivalState::enter() {
    m_score = 0;
    m_runCount = 0;
    m_difficulty = 1;
    m_loading = true;
    m_handlingResult = false;
    m_loadingScreen->enter("Preparing Survival Mode...");
}

void SurvivalState::exit() {
}

void SurvivalState::tick(float dt) {
    if (m_handlingResult) {
        handleRunResult();
        return;
    }

    if (m_loading) {
        m_loadingScreen->tick(dt);
        if (m_loadingScreen->isDone()) {
            m_loading = false;
            startRun();
        }
        return;
    }
}

void SurvivalState::draw(Renderer& renderer) {
    if (m_loading) {
        m_loadingScreen->draw(renderer);
        return;
    }

    int w = renderer.getWidth();
    int h = renderer.getHeight();
    renderer.drawRect({ 0, 0, w, h }, { 15, 15, 30, 255 }, true);

    std::string scoreStr = "Survival Score: " + std::to_string(m_score);
    renderer.drawText(w / 2 - 50, h / 2 - 20, scoreStr, { 255, 255, 255, 255 }, 14);

    std::string runStr = "Runs: " + std::to_string(m_runCount);
    renderer.drawText(w / 2 - 30, h / 2, runStr, { 200, 200, 200, 255 }, 12);

    renderer.drawText(w / 2 - 60, h / 2 + 25, "Press SPACE to start", { 150, 150, 150, 255 }, 10);
}

void SurvivalState::handleInput(const InputState& input) {
    if (m_loading)
        return;
    if (m_handlingResult)
        return;

    if (input.menu_confirm) {
        m_loading = true;
        m_loadingScreen->enter("Loading track...");
    }
    if (input.menu_cancel) {
        m_game->popState();
    }
}

void SurvivalState::startRun() {
    m_runCount++;
    int segCount = 10 + m_difficulty * 3;

    auto track = TrackBuilder().generateProcedural(m_difficulty, segCount);
    auto elements = std::make_unique<TrackElements>();
    TrackBuilder().generateProceduralElements(*elements, *track, m_difficulty);

    auto playState = std::make_unique<PlayState>(*m_game);
    playState->setLevelData(std::move(track), std::move(elements));
    playState->setTrainType(TrainType::Medium);
    playState->setLevelNumber(0);
    m_game->pushState(std::move(playState));
    m_handlingResult = true;
}

void SurvivalState::handleRunResult() {
    auto& result = m_game->getLevelResult();
    if (!result.active)
        return;

    if (!result.completed) {
        // Crashed - accumulate some score
        m_score += result.score / 2;
    } else {
        m_score += result.score;
    }

    m_difficulty++;
    m_game->clearLevelResult();
    m_handlingResult = false;

    // Show loading before next run
    m_loading = true;
    m_loadingScreen->enter("Next track loading...");
}

} // namespace rr99
