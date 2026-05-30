#include "PlayState.h"

#include "HappinessSystem.h"
#include "PassengerSystem.h"
#include "PowerUpManager.h"
#include "ScoreManager.h"
#include "core/EventBus.h"
#include "core/Game.h"
#include "input/InputState.h"
#include "physics/CoasterPhysics.h"
#include "physics/TrackSegment.h"
#include "physics/Train.h"
#include "render/Camera.h"
#include "render/ParticleSystem.h"
#include "render/Renderer.h"
#include "render/WeatherSystem.h"
#include "render/ui/HUD.h"
#include "track/TrackElements.h"
#include "track/TrackLayout.h"

#include <algorithm>
#include <cmath>
#include <string>

namespace rr99 {

PlayState::PlayState(Game& game)
    : m_game(&game)
    , m_physics(std::make_unique<CoasterPhysics>())
    , m_score(std::make_unique<ScoreManager>())
    , m_powerUps(std::make_unique<PowerUpManager>())
    , m_happiness(std::make_unique<HappinessSystem>())
    , m_passengers(std::make_unique<PassengerSystem>())
    , m_camera(std::make_unique<Camera>())
    , m_particles(std::make_unique<ParticleSystem>(128))
    , m_weather(std::make_unique<WeatherSystem>())
    , m_hud(std::make_unique<HUD>()) {
}

PlayState::~PlayState() = default;

void PlayState::setLevelData(std::unique_ptr<TrackLayout> track, std::unique_ptr<TrackElements> elements) {
    m_track = std::move(track);
    m_elements = std::move(elements);
}

void PlayState::enter() {
    if (!m_track)
        return;

    setupRun();

    m_camera->setViewport(m_game->getRenderer().getWidth(), m_game->getRenderer().getHeight());

    if (!m_track->getMetadata().weather.empty()) {
        m_weather->configure(m_track->getMetadata().weather);
    }

    m_score->setParTime(m_track->getMetadata().parTime);
    m_hud->setLevel(m_levelNumber);
}

void PlayState::exit() {
    m_completed = false;
    m_crashed = false;
}

void PlayState::setupRun() {
    m_train = std::make_unique<Train>(m_trainType);
    m_totalTime = 0.0f;
    m_completed = false;
    m_crashed = false;
    m_resultTimer = 0.0f;

    m_score->reset();
    m_powerUps->reset();
    m_happiness->reset();
    m_passengers->reset();
    m_particles->clear();
    m_camera->reset();

    m_passengers->initialize(*m_train);

    // Precompute segment start positions
    m_segmentStartPositions.clear();
    float cumulative = 0.0f;
    m_segmentStartPositions.push_back(0.0f);
    for (int i = 0; i < m_track->getSegmentCount(); ++i) {
        cumulative += m_track->getSegment(i)->getLength();
        m_segmentStartPositions.push_back(cumulative);
    }
}

void PlayState::tick(float dt) {
    if (!m_track || !m_train)
        return;

    if (m_crashed || m_completed) {
        m_resultTimer += dt;
        if (m_resultTimer > 2.0f) {
            m_game->popState();
        }
        return;
    }

    m_totalTime += dt;

    float frontPos = m_train->getCar(0).trackPosition;
    int segIdx = getSegmentAtPosition(frontPos);
    segIdx = std::min(segIdx, m_track->getSegmentCount() - 1);

    TrackSegment* currentSeg = m_track->getSegment(segIdx);
    if (!currentSeg)
        return;

    bool nitroActive = m_powerUps->isNitroActive();
    m_physics->tick(*m_train, *currentSeg, dt, m_inputAccelerating, m_inputBraking, nitroActive);

    // Update power-ups (applies continuous effects)
    m_powerUps->update(dt, *m_train, *m_physics);

    // Check element collection
    checkElementCollection();

    // Check crash
    PhysicsResult result = m_physics->getLastResult();
    if (result.crashed) {
        handleCrash();
        return;
    }

    // Update passengers based on G-force
    float maxGForce = m_physics->calculateGForce(*m_train, *currentSeg);
    m_passengers->update(*m_train, maxGForce);

    // Update happiness based on speed
    float speed = m_train->getCar(0).speed;
    float segMin = currentSeg->getMinSafeSpeed();
    float segMax = currentSeg->getMaxSafeSpeed();
    m_happiness->update(speed, segMin, segMax);

    // Update score
    m_score->updateSpeedBonus(m_happiness->getHappiness(), dt);

    // Update camera
    m_camera->followTrack(*m_track, frontPos, speed);

    // Update particles
    m_particles->update(dt);
    if (m_passengers->hasEjectedThisFrame()) {
        m_particles->emit(ParticleType::Spark, 100.0f, 100.0f, 5);
    }

    // Update weather
    Renderer& renderer = m_game->getRenderer();
    m_weather->update(dt, renderer.getWidth(), renderer.getHeight());

    // Update HUD
    m_hud->setSpeed(speed);
    m_hud->setTokens(m_score->getTokensCollected(), m_elements ? m_elements->getTokenCount() : 0);
    m_hud->setHappiness(m_happiness->getHappiness());
    m_hud->setPassengers(m_train->getRemainingPassengers(), m_train->getTotalPassengers());

    if (m_powerUps->isActive()) {
        m_hud->setPowerUp(
            TrackElements::getPowerUpName(m_powerUps->getActiveType()), m_powerUps->getRemainingTime(),
            m_powerUps->getTotalDuration());
    } else {
        m_hud->clearPowerUp();
    }
    m_hud->setTime(m_totalTime);
    m_hud->setScore(m_score->getScore());

    // Check level completion
    float totalLen = m_track->getTotalLength();
    if (frontPos >= totalLen) {
        handleLevelComplete();
    }
}

void PlayState::checkElementCollection() {
    if (!m_elements || !m_train)
        return;

    float frontPos = m_train->getCar(0).trackPosition;
    float speed = m_train->getCar(0).speed;

    // Check tokens
    for (int i = 0; i < m_elements->getTokenCount(); ++i) {
        SmileToken* token = m_elements->getToken(i);
        if (!token || token->collected)
            continue;

        float tokenPos = getElementGlobalPosition(token->segmentIndex, token->position);
        if (std::abs(frontPos - tokenPos) < 5.0f) {
            // Check speed window
            if (speed >= token->speedMin && speed <= token->speedMax) {
                if (m_elements->collectToken(i, speed)) {
                    m_score->addTokenScore(100);
                    m_particles->emit(ParticleType::Sparkle, 100.0f, 100.0f, 8);
                }
            }
        }
    }

    // Check power-ups
    for (int i = 0; i < m_elements->getPowerUpCount(); ++i) {
        PowerUp* pu = m_elements->getPowerUp(i);
        if (!pu || pu->collected)
            continue;

        float puPos = getElementGlobalPosition(pu->segmentIndex, pu->position);
        if (std::abs(frontPos - puPos) < 5.0f) {
            if (m_elements->collectPowerUp(i)) {
                m_powerUps->activate(pu->type);
                m_particles->emit(ParticleType::Sparkle, 100.0f, 100.0f, 12);
            }
        }
    }
}

void PlayState::handleInput(const InputState& input) {
    if (m_crashed || m_completed)
        return;
    m_inputAccelerating = input.accelerating;
    m_inputBraking = input.braking;
}

void PlayState::handleCrash() {
    m_crashed = true;
    m_resultTimer = 0.0f;
    m_score->calculateFinalScore(
        m_happiness->getScoreMultiplier(m_happiness->getHappiness()), m_passengers->getScoreMultiplier());
    m_particles->emit(ParticleType::Spark, 100.0f, 100.0f, 20);
    m_game->setLevelResult(false, m_levelNumber, m_score->getScore(), m_score->getStarRating());
}

void PlayState::handleLevelComplete() {
    m_completed = true;
    m_resultTimer = 0.0f;
    m_score->setCompletionTime(m_totalTime);
    m_score->calculateFinalScore(
        m_happiness->getScoreMultiplier(m_happiness->getHappiness()), m_passengers->getScoreMultiplier());
    m_game->setLevelResult(true, m_levelNumber, m_score->getScore(), m_score->getStarRating());
}

void PlayState::draw(Renderer& renderer) {
    // Background
    SDL_Rect bg = { 0, 0, renderer.getWidth(), renderer.getHeight() };
    renderer.drawRect(&bg, { 30, 40, 60, 255 }, true);

    // Weather drawn behind everything
    m_weather->draw(renderer);

    // Draw track
    drawTrack(renderer);

    // Draw elements (tokens, power-ups)
    drawElements(renderer);

    // Draw train
    drawTrain(renderer);

    // Draw particles
    m_particles->draw(renderer);

    // Draw HUD
    m_hud->draw(renderer);

    // Draw crash/complete overlay
    if (m_crashed) {
        SDL_Rect overlay = { 0, 0, renderer.getWidth(), renderer.getHeight() };
        renderer.drawRect(&overlay, { 255, 0, 0, 100 }, true);
        renderer.drawText(renderer.getWidth() / 2 - 40, renderer.getHeight() / 2 - 10, "CRASH!", { 255, 255, 255, 255 }, 20);
        std::string scoreStr = "Score: " + std::to_string(m_score->getScore());
        renderer.drawText(renderer.getWidth() / 2 - 40, renderer.getHeight() / 2 + 15, scoreStr, { 255, 255, 255, 255 }, 12);
    } else if (m_completed) {
        SDL_Rect overlay = { 0, 0, renderer.getWidth(), renderer.getHeight() };
        renderer.drawRect(&overlay, { 0, 100, 0, 100 }, true);
        renderer.drawText(
            renderer.getWidth() / 2 - 50, renderer.getHeight() / 2 - 20, "LEVEL COMPLETE!", { 255, 255, 255, 255 }, 16);
        std::string scoreStr = "Score: " + std::to_string(m_score->getScore()) +
            "  Stars: " + std::to_string(m_score->getStarRating());
        renderer.drawText(renderer.getWidth() / 2 - 60, renderer.getHeight() / 2 + 5, scoreStr, { 255, 255, 255, 255 }, 12);
    }
}

void PlayState::drawTrack(Renderer& renderer) {
    if (!m_track)
        return;

    const float SCALE = 2.0f;
    float trackBaseY = 200.0f;

    for (int i = 0; i < m_track->getSegmentCount(); ++i) {
        auto* seg = m_track->getSegment(i);
        if (!seg)
            continue;

        float startX = m_segmentStartPositions[i] * SCALE + 30.0f;
        float endX = m_segmentStartPositions[i + 1] * SCALE + 30.0f;
        float width = endX - startX;
        float y = trackBaseY + seg->getHeightDelta() * SCALE;

        SDL_Color color;
        switch (seg->getType()) {
            case SegmentType::Straight:
                color = { 150, 150, 150, 255 };
                break;
            case SegmentType::Curve:
                color = { 60, 100, 200, 255 };
                break;
            case SegmentType::Drop:
                color = { 200, 50, 50, 255 };
                break;
            case SegmentType::Jump:
                color = { 50, 200, 50, 255 };
                break;
            case SegmentType::Loop:
                color = { 150, 50, 200, 255 };
                break;
            case SegmentType::Tunnel:
                color = { 60, 60, 60, 255 };
                break;
            case SegmentType::WaterSplash:
                color = { 50, 150, 200, 255 };
                break;
            case SegmentType::FallingTrack:
                color = { 200, 120, 20, 255 };
                break;
            default:
                color = { 100, 100, 100, 255 };
                break;
        }

        SDL_Rect segRect = { static_cast<int>(startX), static_cast<int>(y), static_cast<int>(std::max(width, 2.0f)), 8 };
        renderer.drawRect(&segRect, color, true);

        // Border
        renderer.drawRect(&segRect, { 200, 200, 200, 255 }, false);
    }
}

void PlayState::drawTrain(Renderer& renderer) {
    if (!m_train)
        return;

    const float SCALE = 2.0f;
    float trackBaseY = 200.0f;

    for (int i = 0; i < m_train->getCarCount(); ++i) {
        const CarState& car = m_train->getCar(i);

        if (car.derailed)
            continue;

        int segIdx = getSegmentAtPosition(car.trackPosition);
        segIdx = std::min(segIdx, m_track->getSegmentCount() - 1);

        auto* seg = m_track->getSegment(segIdx);

        float worldX = car.trackPosition * SCALE + 30.0f;
        float worldY = trackBaseY + (seg ? seg->getHeightDelta() : 0.0f) * SCALE;

        SDL_Color trainColor;
        switch (m_trainType) {
            case TrainType::Light:
                trainColor = { 255, 200, 50, 255 };
                break;
            case TrainType::Medium:
                trainColor = { 50, 150, 255, 255 };
                break;
            case TrainType::Heavy:
                trainColor = { 200, 50, 50, 255 };
                break;
            default:
                trainColor = { 255, 255, 255, 255 };
                break;
        }

        SDL_Rect carRect = { static_cast<int>(worldX - 5), static_cast<int>(worldY - 6), 10, 12 };
        renderer.drawRect(&carRect, trainColor, true);
        renderer.drawRect(&carRect, { 255, 255, 255, 255 }, false);
    }
}

void PlayState::drawElements(Renderer& renderer) {
    if (!m_elements || !m_track)
        return;

    const float SCALE = 2.0f;
    float trackBaseY = 200.0f;

    // Draw tokens
    for (int i = 0; i < m_elements->getTokenCount(); ++i) {
        const SmileToken* token = m_elements->getToken(i);
        if (!token || token->collected)
            continue;

        float globalPos = getElementGlobalPosition(token->segmentIndex, token->position);
        float x = globalPos * SCALE + 30.0f;

        auto* seg = m_track->getSegment(token->segmentIndex);
        float y = trackBaseY + (seg ? seg->getHeightDelta() * SCALE : 0.0f);

        SDL_Rect tokenRect = { static_cast<int>(x - 4), static_cast<int>(y - 4), 8, 8 };
        renderer.drawRect(&tokenRect, { 255, 255, 0, 255 }, true);
    }

    // Draw power-ups
    for (int i = 0; i < m_elements->getPowerUpCount(); ++i) {
        const PowerUp* pu = m_elements->getPowerUp(i);
        if (!pu || pu->collected)
            continue;

        float globalPos = getElementGlobalPosition(pu->segmentIndex, pu->position);
        float x = globalPos * SCALE + 30.0f;

        auto* seg = m_track->getSegment(pu->segmentIndex);
        float y = trackBaseY + (seg ? seg->getHeightDelta() * SCALE : 0.0f);

        SDL_Color puColor;
        switch (pu->type) {
            case PowerUpType::NitroBoost:
                puColor = { 255, 100, 0, 255 };
                break;
            case PowerUpType::StickyWheels:
                puColor = { 100, 200, 100, 255 };
                break;
            case PowerUpType::Wings:
                puColor = { 100, 150, 255, 255 };
                break;
            case PowerUpType::Magnet:
                puColor = { 200, 100, 200, 255 };
                break;
            case PowerUpType::SlowMotion:
                puColor = { 100, 200, 200, 255 };
                break;
            default:
                puColor = { 255, 255, 255, 255 };
                break;
        }

        SDL_Rect puRect = { static_cast<int>(x - 5), static_cast<int>(y - 5), 10, 10 };
        renderer.drawRect(&puRect, puColor, true);
        renderer.drawRect(&puRect, { 255, 255, 255, 255 }, false);
    }
}

int PlayState::getSegmentAtPosition(float trackPos) const {
    if (trackPos < 0.0f)
        return 0;
    for (int i = 0; i < static_cast<int>(m_segmentStartPositions.size()) - 1; ++i) {
        if (trackPos >= m_segmentStartPositions[i] && trackPos < m_segmentStartPositions[i + 1]) {
            return i;
        }
    }
    return static_cast<int>(m_segmentStartPositions.size()) - 2;
}

float PlayState::getSegmentStart(int index) const {
    if (index < 0)
        return 0.0f;
    if (index >= static_cast<int>(m_segmentStartPositions.size())) {
        return m_segmentStartPositions.empty() ? 0.0f : m_segmentStartPositions.back();
    }
    return m_segmentStartPositions[index];
}

float PlayState::getElementGlobalPosition(int segmentIndex, float position) const {
    float segStart = getSegmentStart(segmentIndex);
    if (segmentIndex >= 0 && segmentIndex < m_track->getSegmentCount()) {
        auto* seg = m_track->getSegment(segmentIndex);
        if (seg) {
            return segStart + position * seg->getLength();
        }
    }
    return segStart + position;
}

} // namespace rr99
