#include "Camera.h"

#include "track/TrackLayout.h"

#include <algorithm>
#include <cmath>

namespace rr99 {

void Camera::followTrack(const TrackLayout& track, float trainPosition, float speed) {
    float totalLen = std::max(track.getTotalLength(), 1.0f);
    float trackProgress = trainPosition / totalLen;

    m_targetZoom = 1.0f;

    if (speed > 60.0f) {
        m_targetZoom = 0.7f;
    } else if (speed < 15.0f) {
        m_targetZoom = 1.3f;
    }

    float lookAhead = (speed / 80.0f) * 50.0f;
    float targetX = trackProgress * 2000.0f - static_cast<float>(m_viewportW) * 0.3f + lookAhead;

    m_x += (targetX - m_x) * 0.1f;
    m_zoom += (m_targetZoom - m_zoom) * 0.05f;
}

void Camera::setViewport(int width, int height) {
    m_viewportW = width;
    m_viewportH = height;
}

void Camera::setZoom(float zoom) {
    m_zoom = zoom;
    m_targetZoom = zoom;
}

SDL_Point Camera::worldToScreen(float worldX, float worldY) const {
    SDL_Point pt;
    pt.x = static_cast<int>((worldX - m_x) * m_zoom + static_cast<float>(m_viewportW) * 0.5f);
    pt.y = static_cast<int>((worldY - m_y) * m_zoom + static_cast<float>(m_viewportH) * 0.5f);
    return pt;
}

float Camera::getParallaxOffset(int layer, float baseX) const {
    float factors[] = { 0.1f, 0.3f, 0.6f, 0.9f };
    int idx = std::min(layer, 3);
    return baseX - m_x * factors[idx];
}

void Camera::reset() {
    m_x = 0.0f;
    m_y = 0.0f;
    m_zoom = 1.0f;
    m_targetZoom = 1.0f;
}

} // namespace rr99
