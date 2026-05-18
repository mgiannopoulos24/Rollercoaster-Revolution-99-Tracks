#include "CoasterPhysics.h"

#include "TrackSegment.h"
#include "Train.h"

#include <algorithm>
#include <cmath>

namespace rr99 {

void CoasterPhysics::tick(
    Train& train, const TrackSegment& segment, float dt, bool accelerating, bool braking, bool nitroActive) {
    m_lastResult = PhysicsResult();

    if (train.isFullyDerailed())
        return;

    applyAcceleration(train, dt, accelerating, braking);
    applyDrag(train, dt);
    applyGravity(train, segment, dt);

    if (nitroActive) {
        applyNitro(train, dt);
    }

    updateCarPositions(train, dt);
    calculateCarGForces(train, segment);
    detectCrashes(train, segment);
}

void CoasterPhysics::applyNitroBoost(Train& train) {
    for (int i = 0; i < train.getCarCount(); ++i) {
        auto& car = train.getCar(i);
        if (!car.derailed) {
            car.speed += NITRO_IMPULSE;
            car.speed = std::min(car.speed, train.getProperties().maxSpeed * 1.3f);
        }
    }
}

void CoasterPhysics::applyGravity(Train& train, const TrackSegment& segment, float dt) {
    float heightDelta = segment.getHeightDelta();
    if (heightDelta != 0.0f) {
        float gravForce = getGravityForce(train.getProperties().mass);
        float speedChange = gravForce * std::copysign(1.0f, -heightDelta) * dt;

        for (int i = 0; i < train.getCarCount(); ++i) {
            auto& car = train.getCar(i);
            if (!car.derailed) {
                car.speed += speedChange / train.getProperties().mass;
                if (car.speed < 0.0f)
                    car.speed = 0.0f;
            }
        }
    }
}

void CoasterPhysics::applyAcceleration(Train& train, float dt, bool accelerating, bool braking) {
    for (int i = 0; i < train.getCarCount(); ++i) {
        auto& car = train.getCar(i);
        if (car.derailed)
            continue;

        if (accelerating) {
            car.speed += train.getProperties().acceleration * dt;
        }
        if (braking) {
            car.speed -= train.getProperties().braking * dt;
        }

        car.speed = std::max(0.0f, std::min(car.speed, train.getProperties().maxSpeed));
    }
}

void CoasterPhysics::applyDrag(Train& train, float dt) {
    for (int i = 0; i < train.getCarCount(); ++i) {
        auto& car = train.getCar(i);
        if (!car.derailed) {
            float dragForce = train.getProperties().drag * car.speed * car.speed;
            car.speed -= dragForce * dt;
            if (car.speed < 0.0f)
                car.speed = 0.0f;
        }
    }
}

void CoasterPhysics::applyNitro(Train& train, float dt) {
    (void)dt;
    for (int i = 0; i < train.getCarCount(); ++i) {
        auto& car = train.getCar(i);
        if (!car.derailed) {
            car.speed += NITRO_IMPULSE * dt;
            car.speed = std::min(car.speed, train.getProperties().maxSpeed * 1.3f);
        }
    }
}

void CoasterPhysics::updateCarPositions(Train& train, float dt) {
    for (int i = 0; i < train.getCarCount(); ++i) {
        auto& car = train.getCar(i);
        if (!car.derailed) {
            car.trackPosition += car.speed * dt;
        }
    }

    for (int i = 1; i < train.getCarCount(); ++i) {
        auto& prevCar = train.getCar(i - 1);
        auto& car = train.getCar(i);
        float minDist = CAR_SPACING;

        if (!car.derailed && !prevCar.derailed) {
            if (car.trackPosition > prevCar.trackPosition - minDist) {
                car.trackPosition = prevCar.trackPosition - minDist;
            }
        }
    }
}

void CoasterPhysics::calculateCarGForces(Train& train, const TrackSegment& segment) {
    for (int i = 0; i < train.getCarCount(); ++i) {
        auto& car = train.getCar(i);
        if (car.derailed)
            continue;

        float speedG = (car.speed * car.speed) / (segment.getLength() + 1.0f) / GRAVITY;
        car.gForce = speedG * segment.getGForceMultiplier();

        if (car.gForce > m_lastResult.maxGForce) {
            m_lastResult.maxGForce = car.gForce;
        }
    }
}

void CoasterPhysics::detectCrashes(Train& train, const TrackSegment& segment) {
    if (train.isFullyDerailed()) {
        m_lastResult.crashed = true;
        m_lastResult.derailed = true;
        return;
    }

    CrashType crashType = segment.getCrashType();
    if (crashType == CrashType::None)
        return;

    for (int i = 0; i < train.getCarCount(); ++i) {
        auto& car = train.getCar(i);
        if (car.derailed)
            continue;

        switch (crashType) {
            case CrashType::Derailment:
                if (car.speed > segment.getMaxSafeSpeed() || car.speed < segment.getMinSafeSpeed()) {
                    car.derailed = true;
                    m_lastResult.derailed = true;
                    m_lastResult.crashed = true;
                }
                break;
            case CrashType::LoopOvershoot:
                if (car.speed < segment.getMinSafeSpeed()) {
                    car.derailed = true;
                    m_lastResult.derailed = true;
                    m_lastResult.loopOvershoot = true;
                    m_lastResult.crashed = true;
                }
                break;
            case CrashType::FallingTrackFailure:
                if (car.speed < segment.getMinSafeSpeed() || car.speed > segment.getMaxSafeSpeed()) {
                    car.derailed = true;
                    m_lastResult.derailed = true;
                    m_lastResult.fallingTrackFailed = true;
                    m_lastResult.crashed = true;
                }
                break;
            case CrashType::WaterSplashBump:
                if (car.speed > segment.getMaxSafeSpeed()) {
                    if (car.passengers > 0) {
                        car.passengers--;
                        m_lastResult.passengerEjected = true;
                    }
                }
                break;
            case CrashType::None:
                break;
        }
    }
}

bool CoasterPhysics::checkCrash(const Train& train, const TrackSegment& segment) const {
    (void)segment;
    return train.isAnyDerailed();
}

float CoasterPhysics::calculateGForce(const Train& train, const TrackSegment& segment) const {
    (void)segment;
    float maxG = 0.0f;
    for (int i = 0; i < train.getCarCount(); ++i) {
        if (train.getCar(i).gForce > maxG) {
            maxG = train.getCar(i).gForce;
        }
    }
    return maxG;
}

float CoasterPhysics::getGravityForce(float mass) {
    return mass * GRAVITY;
}

float CoasterPhysics::getSpeedAfterSegment(
    float entrySpeed, const TrackSegment& segment, float dt, bool accelerating, bool braking) {
    float speed = entrySpeed;
    float friction = segment.getFrictionCoeff() * speed * speed * dt;
    speed -= friction;

    if (accelerating)
        speed += 30.0f * dt;
    if (braking)
        speed -= 40.0f * dt;

    float heightDelta = segment.getHeightDelta();
    if (heightDelta != 0.0f) {
        speed += getGravityForce(1.0f) * std::copysign(1.0f, -heightDelta) * dt;
    }

    return std::max(0.0f, speed);
}

} // namespace rr99
