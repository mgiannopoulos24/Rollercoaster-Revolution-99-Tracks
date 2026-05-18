#ifndef RR99_TRACK_TRACKVALIDATOR_H
#define RR99_TRACK_TRACKVALIDATOR_H

#include <string>
#include <vector>

namespace rr99 {

class TrackLayout;
class TrackElements;

struct ValidationResult {
    bool valid = true;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

class TrackValidator {
public:
    TrackValidator() = default;

    ValidationResult validate(const TrackLayout& layout);
    ValidationResult validateWithElements(const TrackLayout& layout, const TrackElements& elements);

    static bool isCompletable(const TrackLayout& layout);
    static bool areTokensReachable(const TrackLayout& layout, const TrackElements& elements);

private:
    static bool checkConnectivity(const TrackLayout& layout);
    static bool checkNoGaps(const TrackLayout& layout);
    static bool checkReachableFinish(const TrackLayout& layout);
    static bool checkValidLoopSpeeds(const TrackLayout& layout);

    void addError(ValidationResult& result, const std::string& error);
    void addWarning(ValidationResult& result, const std::string& warning);
};

} // namespace rr99

#endif // RR99_TRACK_TRACKVALIDATOR_H
