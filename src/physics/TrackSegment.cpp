#include "TrackSegment.h"

namespace rr99 {

TrackSegment::TrackSegment(SegmentType type, const SegmentProperties& props)
    : m_type(type)
    , m_props(props) {
}

SegmentProperties TrackSegment::getDefaultProperties(SegmentType type) {
    SegmentProperties props;
    switch (type) {
        case SegmentType::Straight:
            props.heightDelta = 0.0f;
            props.length = 50.0f;
            props.frictionCoeff = 0.02f;
            props.minSafeSpeed = 0.0f;
            props.maxSafeSpeed = 100.0f;
            props.gForceMultiplier = 0.0f;
            props.crashType = CrashType::None;
            break;
        case SegmentType::Curve:
            props.heightDelta = 0.0f;
            props.length = 40.0f;
            props.frictionCoeff = 0.04f;
            props.minSafeSpeed = 5.0f;
            props.maxSafeSpeed = 60.0f;
            props.gForceMultiplier = 1.5f;
            props.crashType = CrashType::Derailment;
            break;
        case SegmentType::Drop:
            props.heightDelta = -30.0f;
            props.length = 35.0f;
            props.frictionCoeff = 0.01f;
            props.minSafeSpeed = 0.0f;
            props.maxSafeSpeed = 100.0f;
            props.gForceMultiplier = 0.5f;
            props.crashType = CrashType::None;
            break;
        case SegmentType::Jump:
            props.heightDelta = -10.0f;
            props.length = 25.0f;
            props.frictionCoeff = 0.01f;
            props.minSafeSpeed = 15.0f;
            props.maxSafeSpeed = 80.0f;
            props.gForceMultiplier = 0.3f;
            props.crashType = CrashType::Derailment;
            break;
        case SegmentType::Loop:
            props.heightDelta = 60.0f;
            props.length = 60.0f;
            props.frictionCoeff = 0.05f;
            props.minSafeSpeed = 25.0f;
            props.maxSafeSpeed = 70.0f;
            props.gForceMultiplier = 3.0f;
            props.crashType = CrashType::LoopOvershoot;
            break;
        case SegmentType::Tunnel:
            props.heightDelta = 0.0f;
            props.length = 30.0f;
            props.frictionCoeff = 0.03f;
            props.minSafeSpeed = 0.0f;
            props.maxSafeSpeed = 80.0f;
            props.gForceMultiplier = 0.0f;
            props.crashType = CrashType::None;
            break;
        case SegmentType::WaterSplash:
            props.heightDelta = -5.0f;
            props.length = 20.0f;
            props.frictionCoeff = 0.15f;
            props.minSafeSpeed = 5.0f;
            props.maxSafeSpeed = 60.0f;
            props.gForceMultiplier = 0.5f;
            props.crashType = CrashType::WaterSplashBump;
            break;
        case SegmentType::FallingTrack:
            props.heightDelta = -40.0f;
            props.length = 45.0f;
            props.frictionCoeff = 0.02f;
            props.minSafeSpeed = 10.0f;
            props.maxSafeSpeed = 70.0f;
            props.gForceMultiplier = 1.0f;
            props.crashType = CrashType::FallingTrackFailure;
            break;
        case SegmentType::COUNT:
            break;
    }
    return props;
}

const char* TrackSegment::getTypeName(SegmentType type) {
    switch (type) {
        case SegmentType::Straight:
            return "straight";
        case SegmentType::Curve:
            return "curve";
        case SegmentType::Drop:
            return "drop";
        case SegmentType::Jump:
            return "jump";
        case SegmentType::Loop:
            return "loop";
        case SegmentType::Tunnel:
            return "tunnel";
        case SegmentType::WaterSplash:
            return "water_splash";
        case SegmentType::FallingTrack:
            return "falling_track";
        case SegmentType::COUNT:
            return "unknown";
    }
    return "unknown";
}

} // namespace rr99
