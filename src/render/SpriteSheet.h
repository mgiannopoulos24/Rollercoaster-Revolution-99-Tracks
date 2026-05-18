#ifndef RR99_RENDER_SPRITESHEET_H
#define RR99_RENDER_SPRITESHEET_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

namespace rr99 {

class Renderer;

class SpriteSheet {
public:
    SpriteSheet() = default;
    ~SpriteSheet();

    bool load(Renderer& renderer, const std::string& path, int frameWidth, int frameHeight);
    void unload();

    void drawFrame(Renderer& renderer, int frameIndex, int x, int y,
                   float scale = 1.0f, double angle = 0.0) const;

    // Animation
    void update(float dt);
    void play(int startFrame, int endFrame, float speed, bool loop = true);
    void stop();
    void reset();

    int getCurrentFrame() const { return m_currentFrame; }
    int getFrameCount() const { return m_frameCount; }
    int getFrameWidth() const { return m_frameWidth; }
    int getFrameHeight() const { return m_frameHeight; }
    bool isPlaying() const { return m_playing; }

    SDL_Texture* getTexture() const { return m_texture; }

private:
    SDL_Texture* m_texture = nullptr;
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_frameCount = 0;

    // Animation state
    bool m_playing = false;
    int m_startFrame = 0;
    int m_endFrame = 0;
    float m_speed = 1.0f;
    float m_elapsed = 0.0f;
    int m_currentFrame = 0;
    bool m_loop = true;
};

} // namespace rr99

#endif // RR99_RENDER_SPRITESHEET_H
