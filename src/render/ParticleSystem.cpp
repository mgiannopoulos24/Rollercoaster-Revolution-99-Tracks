#include "ParticleSystem.h"

#include "Renderer.h"
#include "core/Random.h"

#include <algorithm>
#include <cmath>

namespace rr99 {

ParticleSystem::ParticleSystem(int poolSize)
    : m_particles(poolSize)
    , m_poolSize(poolSize) {
}

int ParticleSystem::findAvailable() {
    for (int i = 0; i < m_poolSize; ++i) {
        if (!m_particles[i].active)
            return i;
    }
    return -1;
}

void ParticleSystem::emit(ParticleType type, float x, float y, int count) {
    for (int i = 0; i < count; ++i) {
        int idx = findAvailable();
        if (idx < 0)
            return;

        auto& p = m_particles[idx];
        p.x = x;
        p.y = y;
        p.active = true;

        switch (type) {
            case ParticleType::Spark:
                p.vx = Random::nextFloat() * 100.0f - 50.0f;
                p.vy = Random::nextFloat() * -80.0f - 20.0f;
                p.life = p.maxLife = 0.5f + Random::nextFloat() * 0.5f;
                p.r = 255;
                p.g = 200;
                p.b = 50;
                p.a = 255;
                p.size = 3.0f;
                break;
            case ParticleType::Smoke:
                p.vx = Random::nextFloat() * 20.0f - 10.0f;
                p.vy = -10.0f - Random::nextFloat() * 20.0f;
                p.life = p.maxLife = 1.0f + Random::nextFloat() * 1.0f;
                p.r = 150;
                p.g = 150;
                p.b = 150;
                p.a = 200;
                p.size = 8.0f;
                break;
            case ParticleType::Splash:
                p.vx = Random::nextFloat() * 60.0f - 30.0f;
                p.vy = -40.0f - Random::nextFloat() * 30.0f;
                p.life = p.maxLife = 0.8f + Random::nextFloat() * 0.4f;
                p.r = 100;
                p.g = 180;
                p.b = 255;
                p.a = 200;
                p.size = 4.0f;
                break;
            case ParticleType::Sparkle:
                p.vx = Random::nextFloat() * 30.0f - 15.0f;
                p.vy = Random::nextFloat() * -30.0f;
                p.life = p.maxLife = 0.6f + Random::nextFloat() * 0.4f;
                p.r = 255;
                p.g = 255;
                p.b = 100;
                p.a = 255;
                p.size = 2.0f;
                break;
            case ParticleType::Trail:
                p.vx = Random::nextFloat() * 10.0f - 5.0f;
                p.vy = Random::nextFloat() * 5.0f - 2.0f;
                p.life = p.maxLife = 0.3f + Random::nextFloat() * 0.2f;
                p.r = 200;
                p.g = 200;
                p.b = 200;
                p.a = 150;
                p.size = 2.0f;
                break;
        }
    }
}

void ParticleSystem::update(float dt) {
    for (auto& p : m_particles) {
        if (!p.active)
            continue;
        p.x += p.vx * dt;
        p.y += p.vy * dt;
        p.vy += 100.0f * dt;
        p.life -= dt;
        p.a = static_cast<uint8_t>(255.0f * std::max(0.0f, p.life / p.maxLife));
        if (p.life <= 0.0f) {
            p.active = false;
        }
    }
}

void ParticleSystem::draw(Renderer& renderer) const {
    for (const auto& p : m_particles) {
        if (!p.active)
            continue;
        SDL_Rect r = { static_cast<int>(p.x - p.size * 0.5f), static_cast<int>(p.y - p.size * 0.5f), static_cast<int>(p.size),
                       static_cast<int>(p.size) };
        SDL_Color color = { p.r, p.g, p.b, p.a };
        renderer.drawRect(&r, color, true);
    }
}

void ParticleSystem::clear() {
    for (auto& p : m_particles) {
        p.active = false;
    }
}

} // namespace rr99
