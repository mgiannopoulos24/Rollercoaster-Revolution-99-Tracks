#ifndef RR99_PHYSICS_TRACKSEGMENT_H
#define RR99_PHYSICS_TRACKSEGMENT_H

#include <cstdint>
#include <string>

namespace rr99 {

enum class SegmentType : uint8_t { Straight, Curve, Drop, Jump, Loop, Tunnel, WaterSplash, FallingTrack, COUNT };

enum class CrashType : uint8_t { None, Derailment, LoopOvershoot, FallingTrackFailure, WaterSplashBump };

struct SegmentProperties {
    float heightDelta = 0.0f;
    float length = 0.0f;
    float frictionCoeff = 0.0f;
    float minSafeSpeed = 0.0f;
    float maxSafeSpeed = 0.0f;
    float gForceMultiplier = 1.0f;
    CrashType crashType = CrashType::None;
};

class TrackSegment {
public:
    TrackSegment(SegmentType type, const SegmentProperties& props);

    SegmentType getType() const {
        return m_type;
    }
    const SegmentProperties& getProperties() const {
        return m_props;
    }

    float getHeightDelta() const {
        return m_props.heightDelta;
    }
    float getLength() const {
        return m_props.length;
    }
    float getFrictionCoeff() const {
        return m_props.frictionCoeff;
    }
    float getMinSafeSpeed() const {
        return m_props.minSafeSpeed;
    }
    float getMaxSafeSpeed() const {
        return m_props.maxSafeSpeed;
    }
    float getGForceMultiplier() const {
        return m_props.gForceMultiplier;
    }
    CrashType getCrashType() const {
        return m_props.crashType;
    }

    static SegmentProperties getDefaultProperties(SegmentType type);
    static const char* getTypeName(SegmentType type);

private:
    SegmentType m_type;
    SegmentProperties m_props;
};

} // namespace rr99

#endif // RR99_PHYSICS_TRACKSEGMENT_H
