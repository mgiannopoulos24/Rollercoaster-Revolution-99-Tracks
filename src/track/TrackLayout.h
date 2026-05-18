#ifndef RR99_TRACK_TRACKLAYOUT_H
#define RR99_TRACK_TRACKLAYOUT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace rr99 {

class TrackSegment;
enum class TrainType : uint8_t;

struct TrackMetadata {
    int levelNumber = 0;
    std::string theme = "default";
    float parTime = 60.0f;
    TrainType requiredTrain = static_cast<TrainType>(0);
    std::string weather;
    bool hasRequiredTrain = false;
};

class TrackLayout {
public:
    TrackLayout() = default;

    void addSegment(std::unique_ptr<TrackSegment> segment);
    TrackSegment* getSegment(int index);
    const TrackSegment* getSegment(int index) const;
    int getSegmentCount() const {
        return static_cast<int>(m_segments.size());
    }

    float getTotalLength() const {
        return m_totalLength;
    }
    const TrackMetadata& getMetadata() const {
        return m_metadata;
    }
    void setMetadata(const TrackMetadata& meta) {
        m_metadata = meta;
    }

    using iterator = std::vector<std::unique_ptr<TrackSegment>>::iterator;
    using const_iterator = std::vector<std::unique_ptr<TrackSegment>>::const_iterator;
    iterator begin() {
        return m_segments.begin();
    }
    iterator end() {
        return m_segments.end();
    }
    const_iterator begin() const {
        return m_segments.begin();
    }
    const_iterator end() const {
        return m_segments.end();
    }

    void clear();

private:
    std::vector<std::unique_ptr<TrackSegment>> m_segments;
    TrackMetadata m_metadata;
    float m_totalLength = 0.0f;
};

} // namespace rr99

#endif // RR99_TRACK_TRACKLAYOUT_H
