#include "HotSeatState.h"

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

HotSeatState::HotSeatState(Game& game)
    : m_game(&game) {
}

void HotSeatState::enter() {
    m_currentPlayer = 0;
    m_scores[0] = 0;
    m_scores[1] = 0;
    m_playing = false;
    m_handlingResult = false;
    m_gameOver = false;
}

void HotSeatState::exit() {
}

void HotSeatState::tick(float dt) {
    (void)dt;
    if (m_handlingResult) {
        handleTurnResult();
    }
}

void HotSeatState::draw(Renderer& renderer) {
    int w = renderer.getWidth();
    int h = renderer.getHeight();
    renderer.drawRect({ 0, 0, w, h }, { 20, 20, 40, 255 }, true);

    renderer.drawText(w / 2 - 30, 20, "HOT SEAT", { 255, 200, 50, 255 }, 16);

    if (m_gameOver) {
        renderer.drawText(w / 2 - 40, 60, "GAME OVER!", { 255, 255, 255, 255 }, 14);

        std::string p1 = "Player 1: " + std::to_string(m_scores[0]);
        std::string p2 = "Player 2: " + std::to_string(m_scores[1]);
        renderer.drawText(w / 2 - 40, 90, p1, { 200, 200, 255, 255 }, 12);
        renderer.drawText(w / 2 - 40, 105, p2, { 255, 200, 200, 255 }, 12);

        std::string winner;
        if (m_scores[0] > m_scores[1])
            winner = "Player 1 Wins!";
        else if (m_scores[1] > m_scores[0])
            winner = "Player 2 Wins!";
        else
            winner = "Draw!";
        renderer.drawText(w / 2 - 40, 130, winner, { 255, 255, 0, 255 }, 14);

        renderer.drawText(w / 2 - 60, h - 25, "SPACE: Play Again  ESC: Back", { 150, 150, 150, 255 }, 10);
        return;
    }

    if (m_playing)
        return;

    std::string turnStr = "Player " + std::to_string(m_currentPlayer + 1) + "'s Turn";
    renderer.drawText(w / 2 - 40, 60, turnStr, { 255, 255, 255, 255 }, 14);

    std::string p1 = "Player 1 Score: " + std::to_string(m_scores[0]);
    std::string p2 = "Player 2 Score: " + std::to_string(m_scores[1]);
    renderer.drawText(w / 2 - 40, 90, p1, { 200, 200, 255, 255 }, 12);
    renderer.drawText(w / 2 - 40, 105, p2, { 255, 200, 200, 255 }, 12);

    renderer.drawText(w / 2 - 50, h - 25, "SPACE: Start Turn  ESC: Back", { 150, 150, 150, 255 }, 10);
}

void HotSeatState::handleInput(const InputState& input) {
    if (m_handlingResult || m_playing)
        return;

    if (input.menu_confirm) {
        if (m_gameOver) {
            // Restart
            enter();
        } else {
            startPlayerTurn();
        }
    }
    if (input.menu_cancel) {
        m_game->popState();
    }
}

void HotSeatState::startPlayerTurn() {
    m_playing = true;

    auto track = TrackBuilder().generateProcedural(1, 12);
    auto elements = std::make_unique<TrackElements>();
    TrackBuilder().generateProceduralElements(*elements, *track, 1);

    auto playState = std::make_unique<PlayState>(*m_game);
    playState->setLevelData(std::move(track), std::move(elements));
    playState->setTrainType(m_currentPlayer == 0 ? TrainType::Light : TrainType::Heavy);
    playState->setLevelNumber(m_currentPlayer + 1);
    m_game->pushState(std::move(playState));
    m_handlingResult = true;
}

void HotSeatState::handleTurnResult() {
    auto& result = m_game->getLevelResult();
    if (!result.active)
        return;

    m_scores[m_currentPlayer] = result.score;
    m_game->clearLevelResult();
    m_handlingResult = false;
    m_playing = false;

    if (m_currentPlayer == 0) {
        m_currentPlayer = 1;
    } else {
        m_gameOver = true;
    }
}

} // namespace rr99
