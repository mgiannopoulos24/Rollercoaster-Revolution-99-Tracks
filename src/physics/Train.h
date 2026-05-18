#ifndef RR99_PHYSICS_TRAIN_H
#define RR99_PHYSICS_TRAIN_H

#include <cstdint>
#include <vector>

namespace rr99 {

enum class TrainType : uint8_t { Light, Medium, Heavy };

struct TrainProperties {
    float mass = 1.0f;
    float drag = 0.02f;
    float maxSpeed = 80.0f;
    float grip = 1.0f;
    float acceleration = 30.0f;
    float braking = 40.0f;
    int carCount = 3;
    int passengersPerCar = 2;
};

struct CarState {
    float trackPosition = 0.0f;
    float speed = 0.0f;
    float gForce = 0.0f;
    bool derailed = false;
    int passengers = 0;
};

class Train {
public:
    Train(TrainType type);

    TrainType getType() const {
        return m_type;
    }
    const TrainProperties& getProperties() const {
        return m_props;
    }

    CarState& getCar(int index);
    const CarState& getCar(int index) const;
    int getCarCount() const {
        return static_cast<int>(m_cars.size());
    }

    bool isAnyDerailed() const;
    bool isFullyDerailed() const;
    int getTotalPassengers() const;
    int getRemainingPassengers() const;
    void ejectPassenger(int carIndex);

    void setCarCount(int count);
    void reset(const TrainProperties& props);

    static TrainProperties getDefaultProperties(TrainType type);

private:
    TrainType m_type;
    TrainProperties m_props;
    std::vector<CarState> m_cars;
};

} // namespace rr99

#endif // RR99_PHYSICS_TRAIN_H
