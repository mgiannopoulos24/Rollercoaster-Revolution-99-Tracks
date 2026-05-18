#include "TrackValidator.h"

#include "TrackElements.h"
#include "TrackLayout.h"
#include "physics/CoasterPhysics.h"
#include "physics/TrackSegment.h"

namespace rr99 {

ValidationResult TrackValidator::validate(const TrackLayout& layout) {
    ValidationResult result;

    if (layout.getSegmentCount() == 0) {
        addError(result, "Track has no segments");
        return result;
    }

    if (!checkConnectivity(layout)) {
        addError(result, "Track segments are not properly connected");
    }

    if (!checkNoGaps(layout)) {
        addError(result, "Track has gaps between segments");
    }

    if (!checkReachableFinish(layout)) {
        addError(result, "Track finish line is not reachable");
    }

    if (!checkValidLoopSpeeds(layout)) {
        addWarning(result, "Some loops may be impossible to complete at any speed");
    }

    for (int i = 0; i < layout.getSegmentCount(); ++i) {
        auto* seg = layout.getSegment(i);
        if (!seg) {
            addError(result, "Null segment found at index " + std::to_string(i));
            continue;
        }

        if (seg->getLength() <= 0.0f) {
            addError(result, "Segment " + std::to_string(i) + " has zero or negative length");
        }

        if (seg->getMaxSafeSpeed() < seg->getMinSafeSpeed()) {
            addError(result, "Segment " + std::to_string(i) + " has max speed less than min speed");
        }
    }

    result.valid = result.errors.empty();
    return result;
}

ValidationResult TrackValidator::validateWithElements(const TrackLayout& layout, const TrackElements& elements) {
    ValidationResult result = validate(layout);
    if (!result.valid)
        return result;

    for (int i = 0; i < elements.getTokenCount(); ++i) {
        auto* token = elements.getToken(i);
        if (!token)
            continue;

        if (token->segmentIndex < 0 || token->segmentIndex >= layout.getSegmentCount()) {
            addWarning(result, "Token " + std::to_string(i) + " references invalid segment");
            continue;
        }

        if (token->speedMax < token->speedMin) {
            addWarning(result, "Token " + std::to_string(i) + " has speed window where max < min");
        }

        auto* seg = layout.getSegment(token->segmentIndex);
        if (seg) {
            if (token->speedMin < seg->getMinSafeSpeed()) {
                addWarning(result, "Token " + std::to_string(i) + " min speed below segment safe minimum");
            }
            if (token->speedMax > seg->getMaxSafeSpeed()) {
                addWarning(result, "Token " + std::to_string(i) + " max speed above segment safe maximum");
            }
        }
    }

    for (int i = 0; i < elements.getPowerUpCount(); ++i) {
        auto* pu = elements.getPowerUp(i);
        if (!pu)
            continue;

        if (pu->segmentIndex < 0 || pu->segmentIndex >= layout.getSegmentCount()) {
            addWarning(result, "Power-up " + std::to_string(i) + " references invalid segment");
        }
    }

    for (int i = 0; i < elements.getHazardCount(); ++i) {
        auto* hazard = elements.getHazard(i);
        if (!hazard)
            continue;

        if (hazard->segmentIndex < 0 || hazard->segmentIndex >= layout.getSegmentCount()) {
            addWarning(result, "Hazard " + std::to_string(i) + " references invalid segment");
        }
    }

    if (!areTokensReachable(layout, elements)) {
        addWarning(result, "Some tokens may be unreachable at any valid speed");
    }

    result.valid = result.errors.empty();
    return result;
}

bool TrackValidator::isCompletable(const TrackLayout& layout) {
    return checkReachableFinish(layout) && checkValidLoopSpeeds(layout);
}

bool TrackValidator::areTokensReachable(const TrackLayout& layout, const TrackElements& elements) {
    for (int i = 0; i < elements.getTokenCount(); ++i) {
        auto* token = elements.getToken(i);
        if (!token || token->collected)
            continue;

        if (token->segmentIndex < 0 || token->segmentIndex >= layout.getSegmentCount()) {
            return false;
        }

        auto* seg = layout.getSegment(token->segmentIndex);
        if (!seg)
            continue;

        float segMin = seg->getMinSafeSpeed();
        float segMax = seg->getMaxSafeSpeed();

        if (token->speedMin > segMax || token->speedMax < segMin) {
            return false;
        }
    }
    return true;
}

bool TrackValidator::checkConnectivity(const TrackLayout& layout) {
    (void)layout;
    (void)layout;
    return layout.getSegmentCount() > 0;
}

bool TrackValidator::checkNoGaps(const TrackLayout& layout) {
    for (int i = 0; i < layout.getSegmentCount(); ++i) {
        auto* seg = layout.getSegment(i);
        if (!seg || seg->getLength() <= 0.0f) {
            return false;
        }
    }
    return true;
}

bool TrackValidator::checkReachableFinish(const TrackLayout& layout) {
    float simSpeed = 30.0f;
    bool stuck = false;

    for (int i = 0; i < layout.getSegmentCount(); ++i) {
        auto* seg = layout.getSegment(i);
        if (!seg)
            return false;

        float drag = seg->getFrictionCoeff() * simSpeed * simSpeed;
        simSpeed -= drag;

        float gravityEffect = CoasterPhysics::GRAVITY;
        if (seg->getHeightDelta() < 0.0f) {
            simSpeed += gravityEffect * std::abs(seg->getHeightDelta()) / seg->getLength();
        } else if (seg->getHeightDelta() > 0.0f) {
            simSpeed -= gravityEffect * seg->getHeightDelta() / seg->getLength();
        }

        if (simSpeed < 0.0f) {
            stuck = true;
            break;
        }

        if (seg->getType() == SegmentType::Jump && simSpeed < seg->getMinSafeSpeed()) {
            return false;
        }
        if (seg->getType() == SegmentType::Loop && simSpeed < seg->getMinSafeSpeed()) {
            return false;
        }
    }

    return !stuck;
}

bool TrackValidator::checkValidLoopSpeeds(const TrackLayout& layout) {
    bool allLoopsValid = true;
    for (int i = 0; i < layout.getSegmentCount(); ++i) {
        auto* seg = layout.getSegment(i);
        if (seg && seg->getType() == SegmentType::Loop) {
            if (seg->getMinSafeSpeed() <= 0.0f || seg->getMaxSafeSpeed() <= seg->getMinSafeSpeed()) {
                allLoopsValid = false;
            }
        }
    }
    return allLoopsValid;
}

void TrackValidator::addError(ValidationResult& result, const std::string& error) {
    result.errors.push_back(error);
    result.valid = false;
}

void TrackValidator::addWarning(ValidationResult& result, const std::string& warning) {
    result.warnings.push_back(warning);
}

} // namespace rr99
