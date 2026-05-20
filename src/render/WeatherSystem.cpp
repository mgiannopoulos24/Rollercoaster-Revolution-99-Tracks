#include "WeatherSystem.h"

#include "Renderer.h"
#include "core/Random.h"

#include <algorithm>
#include <cmath>

namespace rr99 {

void WeatherSystem::configure(const std::string& weatherType) {
    reset();
    m_weatherType = weatherType;
    m_active = !weatherType.empty() && weatherType != "none" && weatherType != "clear";
}

void WeatherSystem::update(float dt, int viewportW, int viewportH) {
    if (!m_active)
        return;

    if (m_weatherType == "rain") {
        if (static_cast<int>(m_rain.size()) < 200) {
            emitRain(5, viewportW, viewportH);
        }
        for (auto& r : m_rain) {
            r.y += r.speed * dt;
            r.x -= 20.0f * dt;
            if (r.y > static_cast<float>(viewportH) + 10.0f) {
                r.y = -10.0f;
                r.x = Random::nextFloat() * static_cast<float>(viewportW);
            }
        }

        m_lightningInterval -= dt;
        if (m_lightningInterval <= 0.0f) {
            m_lightningTimer = 0.15f;
            m_lightningInterval = 5.0f + Random::nextFloat() * 15.0f;
        }
    } else if (m_weatherType == "snow") {
        if (static_cast<int>(m_snow.size()) < 150) {
            emitSnow(3, viewportW, viewportH);
        }
        for (auto& s : m_snow) {
            s.y += s.speed * dt;
            s.x += std::sin(s.y * 0.01f) * s.drift * dt;
            if (s.y > static_cast<float>(viewportH) + 10.0f) {
                s.y = -10.0f;
                s.x = Random::nextFloat() * static_cast<float>(viewportW);
            }
        }
    }

    if (m_lightningTimer > 0.0f) {
        m_lightningTimer -= dt;
    }
}

void WeatherSystem::draw(Renderer& renderer) const {
    if (!m_active)
        return;

    if (m_lightningTimer > 0.0f) {
        SDL_Rect fullRect = { 0, 0, renderer.getWidth(), renderer.getHeight() };
        SDL_Color flash = { 255, 255, 255, 80 };
        renderer.drawRect(&fullRect, flash, true);
    }

    if (m_weatherType == "rain") {
        for (const auto& r : m_rain) {
            SDL_Rect rect = { static_cast<int>(r.x), static_cast<int>(r.y), 1, static_cast<int>(r.length) };
            SDL_Color color = { 180, 200, 255, 180 };
            renderer.drawRect(&rect, color, true);
        }
    } else if (m_weatherType == "snow") {
        for (const auto& s : m_snow) {
            SDL_Rect rect = { static_cast<int>(s.x), static_cast<int>(s.y), static_cast<int>(s.size),
                              static_cast<int>(s.size) };
            SDL_Color color = { 255, 255, 255, 220 };
            renderer.drawRect(&rect, color, true);
        }
    }
}

void WeatherSystem::reset() {
    m_rain.clear();
    m_snow.clear();
    m_active = false;
    m_lightningTimer = 0.0f;
    m_lightningInterval = 0.0f;
    m_weatherType.clear();
}

void WeatherSystem::emitRain(int count, int viewportW, int viewportH) {
    for (int i = 0; i < count; ++i) {
        RainDrop r;
        r.x = Random::nextFloat() * static_cast<float>(viewportW);
        r.y = -Random::nextFloat() * static_cast<float>(viewportH);
        r.speed = 300.0f + Random::nextFloat() * 200.0f;
        r.length = 8.0f + Random::nextFloat() * 8.0f;
        m_rain.push_back(r);
    }
}

void WeatherSystem::emitSnow(int count, int viewportW, int viewportH) {
    for (int i = 0; i < count; ++i) {
        SnowFlake s;
        s.x = Random::nextFloat() * static_cast<float>(viewportW);
        s.y = -Random::nextFloat() * static_cast<float>(viewportH);
        s.speed = 30.0f + Random::nextFloat() * 40.0f;
        s.drift = 10.0f + Random::nextFloat() * 20.0f;
        s.size = 2.0f + Random::nextFloat() * 3.0f;
        m_snow.push_back(s);
    }
}

} // namespace rr99
