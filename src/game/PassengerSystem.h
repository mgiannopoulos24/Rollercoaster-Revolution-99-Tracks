#ifndef RR99_GAME_PASSENGERSYSTEM_H
#define RR99_GAME_PASSENGERSYSTEM_H

namespace rr99 {

class Train;

class PassengerSystem {
public:
    PassengerSystem() = default;

    void initialize(Train& train);
    void update(Train& train, float gForce);

    int getRemainingPassengers() const {
        return m_remainingPassengers;
    }
    int getTotalPassengers() const {
        return m_totalPassengers;
    }
    float getScoreMultiplier() const;
    bool hasEjectedThisFrame() const {
        return m_ejectedThisFrame;
    }
    void reset();

private:
    int m_totalPassengers = 0;
    int m_remainingPassengers = 0;
    bool m_ejectedThisFrame = false;
};

} // namespace rr99

#endif // RR99_GAME_PASSENGERSYSTEM_H
