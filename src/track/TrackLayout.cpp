#include "TrackLayout.h"

#include "physics/TrackSegment.h"

namespace rr99 {

void TrackLayout::addSegment(std::unique_ptr<TrackSegment> segment) {
    m_totalLength += segment->getLength();
    m_segments.push_back(std::move(segment));
}

TrackSegment* TrackLayout::getSegment(int index) {
    if (index < 0 || index >= static_cast<int>(m_segments.size()))
        return nullptr;
    return m_segments[index].get();
}

const TrackSegment* TrackLayout::getSegment(int index) const {
    if (index < 0 || index >= static_cast<int>(m_segments.size()))
        return nullptr;
    return m_segments[index].get();
}

void TrackLayout::clear() {
    m_segments.clear();
    m_totalLength = 0.0f;
    m_metadata = TrackMetadata();
}

} // namespace rr99
