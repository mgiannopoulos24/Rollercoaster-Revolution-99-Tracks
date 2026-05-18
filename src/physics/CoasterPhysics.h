#ifndef RR99_PHYSICS_COASTERPHYSICS_H
#define RR99_PHYSICS_COASTERPHYSICS_H

namespace rr99 {

class Train;
class TrackSegment;

struct PhysicsResult {
    bool crashed = false;
    bool derailed = false;
    bool loopOvershoot = false;
    bool fallingTrackFailed = false;
    bool passengerEjected = false;
    float maxGForce = 0.0f;
};

class CoasterPhysics {
public:
    static constexpr float GRAVITY = 9.81f;
    static constexpr float NITRO_IMPULSE = 30.0f;
    static constexpr float CAR_SPACING = 8.0f;

    void tick(Train& train, const TrackSegment& segment, float dt, bool accelerating, bool braking, bool nitroActive);

    void applyNitroBoost(Train& train);
    PhysicsResult getLastResult() const {
        return m_lastResult;
    }

    bool checkCrash(const Train& train, const TrackSegment& segment) const;
    float calculateGForce(const Train& train, const TrackSegment& segment) const;
    static float getGravityForce(float mass);
    static float getSpeedAfterSegment(float entrySpeed, const TrackSegment& segment, float dt, bool accelerating, bool braking);

private:
    void applyGravity(Train& train, const TrackSegment& segment, float dt);
    void applyAcceleration(Train& train, float dt, bool accelerating, bool braking);
    void applyDrag(Train& train, float dt);
    void applyNitro(Train& train, float dt);
    void updateCarPositions(Train& train, float dt);
    void detectCrashes(Train& train, const TrackSegment& segment);
    void calculateCarGForces(Train& train, const TrackSegment& segment);

    PhysicsResult m_lastResult;
};

} // namespace rr99

#endif // RR99_PHYSICS_COASTERPHYSICS_H
