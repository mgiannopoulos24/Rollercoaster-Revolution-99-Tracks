#ifndef RR99_RENDER_PARTICLESYSTEM_H
#define RR99_RENDER_PARTICLESYSTEM_H

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>

namespace rr99 {

class Renderer;

struct Particle {
    float x = 0.0f, y = 0.0f;
    float vx = 0.0f, vy = 0.0f;
    float life = 1.0f;
    float maxLife = 1.0f;
    uint8_t r = 255, g = 255, b = 255, a = 255;
    float size = 4.0f;
    bool active = false;
};

enum class ParticleType : uint8_t { Spark, Smoke, Splash, Sparkle, Trail };

class ParticleSystem {
public:
    explicit ParticleSystem(int poolSize = 256);

    void emit(ParticleType type, float x, float y, int count = 1);
    void update(float dt);
    void draw(Renderer& renderer) const;
    void clear();

private:
    int findAvailable();

    std::vector<Particle> m_particles;
    int m_poolSize;
};

} // namespace rr99

#endif // RR99_RENDER_PARTICLESYSTEM_H
