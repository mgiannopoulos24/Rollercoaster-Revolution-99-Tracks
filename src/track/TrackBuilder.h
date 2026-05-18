#ifndef RR99_TRACK_TRACKBUILDER_H
#define RR99_TRACK_TRACKBUILDER_H

#include <memory>
#include <string>
#include <vector>

namespace rr99 {

class TrackLayout;
class TrackElements;
enum class TrainType : uint8_t;
enum class PowerUpType : uint8_t;

struct TrackDefinition {
    int levelNumber = 0;
    std::string theme = "default";
    float parTime = 60.0f;
    std::string weather;
    int requiredTrainType = -1;
    std::vector<int> segmentTypes;
    std::vector<float> segmentHeights;
};

class TrackBuilder {
public:
    TrackBuilder() = default;

    std::unique_ptr<TrackLayout> loadFromFile(const std::string& path);
    std::unique_ptr<TrackLayout> loadFromDefinition(const TrackDefinition& def);
    bool saveToFile(const TrackLayout& layout, const std::string& path);

    std::unique_ptr<TrackLayout> generateProcedural(int level, int segmentCount);
    void generateProceduralElements(TrackElements& elements, const TrackLayout& layout, int difficulty);

    static TrackDefinition generateDefinition(int level, int segmentCount);

private:
    int parseSegmentType(const std::string& typeName) const;
};

} // namespace rr99

#endif // RR99_TRACK_TRACKBUILDER_H
