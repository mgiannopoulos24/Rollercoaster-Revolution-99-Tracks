#ifndef RR99_RENDER_WEATHERSYSTEM_H
#define RR99_RENDER_WEATHERSYSTEM_H

#include <string>
#include <vector>

namespace rr99 {

class Renderer;

struct RainDrop {
    float x = 0.0f, y = 0.0f;
    float speed = 300.0f;
    float length = 8.0f;
};

struct SnowFlake {
    float x = 0.0f, y = 0.0f;
    float speed = 30.0f;
    float drift = 10.0f;
    float size = 3.0f;
};

class WeatherSystem {
public:
    WeatherSystem() = default;

    void configure(const std::string& weatherType);
    void update(float dt, int viewportW, int viewportH);
    void draw(Renderer& renderer) const;

    bool isActive() const {
        return m_active;
    }
    const std::string& getType() const {
        return m_weatherType;
    }
    bool isLightningVisible() const {
        return m_lightningTimer > 0.0f;
    }

    void reset();

private:
    void emitRain(int count, int viewportW, int viewportH);
    void emitSnow(int count, int viewportW, int viewportH);

    std::string m_weatherType;
    bool m_active = false;

    std::vector<RainDrop> m_rain;
    std::vector<SnowFlake> m_snow;

    float m_lightningTimer = 0.0f;
    float m_lightningInterval = 0.0f;
};

} // namespace rr99

#endif // RR99_RENDER_WEATHERSYSTEM_H
