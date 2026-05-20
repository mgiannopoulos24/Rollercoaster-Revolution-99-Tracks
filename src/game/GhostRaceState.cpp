#include "GhostRaceState.h"

#include "PlayState.h"
#include "core/Game.h"
#include "input/InputState.h"
#include "physics/TrackSegment.h"
#include "render/Renderer.h"
#include "track/TrackBuilder.h"
#include "track/TrackElements.h"
#include "track/TrackLayout.h"

#include <string>

namespace rr99 {

GhostRaceState::GhostRaceState(Game& game)
    : m_game(&game) {
}

void GhostRaceState::enter() {
    m_selectedLevel = 0;
    m_handlingResult = false;
    m_showResult = false;
    m_lastScore = 0;
}

void GhostRaceState::exit() {
}

void GhostRaceState::tick(float dt) {
    (void)dt;
    if (m_handlingResult) {
        handleRaceResult();
    }
}

void GhostRaceState::draw(Renderer& renderer) {
    int w = renderer.getWidth();
    int h = renderer.getHeight();
    renderer.drawRect({ 0, 0, w, h }, { 20, 20, 40, 255 }, true);

    renderer.drawText(w / 2 - 40, 20, "GHOST RACE", { 100, 200, 255, 255 }, 16);

    if (m_showResult) {
        std::string scoreStr = "Score: " + std::to_string(m_lastScore);
        renderer.drawText(w / 2 - 30, 70, scoreStr, { 255, 255, 255, 255 }, 14);
        renderer.drawText(w / 2 - 70, h - 25, "SPACE: Race Again  ESC: Back", { 150, 150, 150, 255 }, 10);
        return;
    }

    renderer.drawText(w / 2 - 50, 60, "Select a level to race:", { 200, 200, 200, 255 }, 12);

    std::string levelStr = "Level " + std::to_string(m_selectedLevel + 1);
    renderer.drawText(w / 2 - 30, 90, levelStr, { 255, 255, 255, 255 }, 14);

    renderer.drawText(w / 2 - 70, h - 40, "UP/DOWN: Change Level  SPACE: Race", { 150, 150, 150, 255 }, 10);
    renderer.drawText(w / 2 - 30, h - 25, "ESC: Back", { 150, 150, 150, 255 }, 10);
}

void GhostRaceState::handleInput(const InputState& input) {
    if (m_handlingResult)
        return;

    if (m_showResult) {
        if (input.menu_confirm) {
            m_showResult = false;
        }
        if (input.menu_cancel) {
            m_game->popState();
        }
        return;
    }

    if (input.menu_up) {
        m_selectedLevel = std::max(0, m_selectedLevel - 1);
    }
    if (input.menu_down) {
        m_selectedLevel = std::min(98, m_selectedLevel + 1);
    }
    if (input.menu_confirm) {
        startRace(m_selectedLevel);
    }
    if (input.menu_cancel) {
        m_game->popState();
    }
}

void GhostRaceState::startRace(int level) {
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

void GhostRaceState::handleRaceResult() {
    auto& result = m_game->getLevelResult();
    if (!result.active)
        return;

    m_lastScore = result.score;
    m_game->clearLevelResult();
    m_handlingResult = false;
    m_showResult = true;
}

} // namespace rr99
