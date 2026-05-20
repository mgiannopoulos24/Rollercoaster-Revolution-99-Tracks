#ifndef RR99_RENDER_CAMERA_H
#define RR99_RENDER_CAMERA_H

#include <SDL2/SDL.h>

namespace rr99 {

class TrackLayout;

class Camera {
public:
    Camera() = default;

    void followTrack(const TrackLayout& track, float trainPosition, float speed);
    void setViewport(int width, int height);
    void setZoom(float zoom);

    float getX() const {
        return m_x;
    }
    float getY() const {
        return m_y;
    }
    float getZoom() const {
        return m_zoom;
    }

    SDL_Point worldToScreen(float worldX, float worldY) const;
    float getParallaxOffset(int layer, float baseX) const;

    void reset();

private:
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_zoom = 1.0f;
    float m_targetZoom = 1.0f;
    int m_viewportW = 240;
    int m_viewportH = 320;
};

} // namespace rr99

#endif // RR99_RENDER_CAMERA_H
