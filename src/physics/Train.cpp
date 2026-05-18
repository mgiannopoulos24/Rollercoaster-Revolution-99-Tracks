#include "Train.h"

namespace rr99 {

Train::Train(TrainType type)
    : m_type(type)
    , m_props(getDefaultProperties(type)) {
    setCarCount(m_props.carCount);
}

TrainProperties Train::getDefaultProperties(TrainType type) {
    TrainProperties props;
    switch (type) {
        case TrainType::Light:
            props.mass = 0.5f;
            props.drag = 0.015f;
            props.maxSpeed = 100.0f;
            props.grip = 0.6f;
            props.acceleration = 45.0f;
            props.braking = 35.0f;
            props.carCount = 3;
            props.passengersPerCar = 2;
            break;
        case TrainType::Medium:
            props.mass = 1.0f;
            props.drag = 0.020f;
            props.maxSpeed = 80.0f;
            props.grip = 0.8f;
            props.acceleration = 30.0f;
            props.braking = 40.0f;
            props.carCount = 4;
            props.passengersPerCar = 2;
            break;
        case TrainType::Heavy:
            props.mass = 1.8f;
            props.drag = 0.030f;
            props.maxSpeed = 65.0f;
            props.grip = 1.0f;
            props.acceleration = 20.0f;
            props.braking = 50.0f;
            props.carCount = 4;
            props.passengersPerCar = 3;
            break;
    }
    return props;
}

CarState& Train::getCar(int index) {
    return m_cars[index];
}

const CarState& Train::getCar(int index) const {
    return m_cars[index];
}

void Train::setCarCount(int count) {
    m_cars.resize(count);
    for (int i = 0; i < count; ++i) {
        m_cars[i].trackPosition = static_cast<float>(i) * -8.0f;
        m_cars[i].passengers = m_props.passengersPerCar;
    }
}

bool Train::isAnyDerailed() const {
    for (auto& car : m_cars) {
        if (car.derailed)
            return true;
    }
    return false;
}

bool Train::isFullyDerailed() const {
    for (auto& car : m_cars) {
        if (!car.derailed)
            return false;
    }
    return !m_cars.empty();
}

int Train::getTotalPassengers() const {
    return m_props.passengersPerCar * static_cast<int>(m_cars.size());
}

int Train::getRemainingPassengers() const {
    int count = 0;
    for (auto& car : m_cars) {
        count += car.passengers;
    }
    return count;
}

void Train::ejectPassenger(int carIndex) {
    if (carIndex >= 0 && carIndex < static_cast<int>(m_cars.size())) {
        if (m_cars[carIndex].passengers > 0) {
            m_cars[carIndex].passengers--;
        }
    }
}

void Train::reset(const TrainProperties& props) {
    m_props = props;
    m_cars.clear();
    setCarCount(props.carCount);
    for (auto& car : m_cars) {
        car.derailed = false;
    }
}

} // namespace rr99
