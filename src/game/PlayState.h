#ifndef RR99_GAME_PLAYSTATE_H
#define RR99_GAME_PLAYSTATE_H

#include "core/GameState.h"
#include "physics/Train.h"

#include <memory>
#include <vector>

namespace rr99 {

class Game;
class TrackLayout;
class TrackElements;
class CoasterPhysics;
class ScoreManager;
class PowerUpManager;
class HappinessSystem;
class PassengerSystem;
class Camera;
class ParticleSystem;
class WeatherSystem;
class HUD;

class PlayState : public GameState {
public:
    explicit PlayState(Game& game);
    ~PlayState() override;

    void enter() override;
    void exit() override;
    void tick(float dt) override;
    void draw(Renderer& renderer) override;
    void handleInput(const InputState& input) override;

    void setLevelData(std::unique_ptr<TrackLayout> track, std::unique_ptr<TrackElements> elements);
    void setTrainType(TrainType type) {
        m_trainType = type;
    }
    void setLevelNumber(int level) {
        m_levelNumber = level;
    }

private:
    void handleCrash();
    void handleLevelComplete();
    void setupRun();
    void checkElementCollection();
    int getSegmentAtPosition(float trackPos) const;
    float getSegmentStart(int index) const;
    float getElementGlobalPosition(int segmentIndex, float position) const;
    void drawTrack(Renderer& renderer);
    void drawTrain(Renderer& renderer);
    void drawElements(Renderer& renderer);

    Game* m_game;
    bool m_inputAccelerating = false;
    bool m_inputBraking = false;

    std::unique_ptr<TrackLayout> m_track;
    std::unique_ptr<TrackElements> m_elements;
    std::unique_ptr<Train> m_train;
    std::unique_ptr<CoasterPhysics> m_physics;

    std::unique_ptr<ScoreManager> m_score;
    std::unique_ptr<PowerUpManager> m_powerUps;
    std::unique_ptr<HappinessSystem> m_happiness;
    std::unique_ptr<PassengerSystem> m_passengers;

    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<ParticleSystem> m_particles;
    std::unique_ptr<WeatherSystem> m_weather;
    std::unique_ptr<HUD> m_hud;

    TrainType m_trainType = TrainType::Medium;
    int m_levelNumber = 0;
    float m_totalTime = 0.0f;
    bool m_completed = false;
    bool m_crashed = false;
    float m_resultTimer = 0.0f;

    std::vector<float> m_segmentStartPositions;
};

} // namespace rr99

#endif // RR99_GAME_PLAYSTATE_H
