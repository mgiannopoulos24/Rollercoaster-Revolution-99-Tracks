#include "SpriteSheet.h"

#include "Renderer.h"

namespace rr99 {

SpriteSheet::~SpriteSheet() {
    unload();
}

bool SpriteSheet::load(Renderer& renderer, const std::string& path, int frameWidth, int frameHeight) {
    unload();
    m_texture = renderer.loadTexture(path);
    if (!m_texture)
        return false;

    int texWidth, texHeight;
    SDL_QueryTexture(m_texture, nullptr, nullptr, &texWidth, &texHeight);

    m_frameWidth = frameWidth;
    m_frameHeight = frameHeight;
    m_frameCount = (texWidth / frameWidth) * (texHeight / frameHeight);

    return true;
}

void SpriteSheet::unload() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    m_frameCount = 0;
    m_playing = false;
    m_currentFrame = 0;
}

void SpriteSheet::drawFrame(Renderer& renderer, int frameIndex, int x, int y, float scale, double angle) const {
    if (!m_texture || frameIndex < 0 || frameIndex >= m_frameCount)
        return;

    int cols = 0;
    SDL_QueryTexture(m_texture, nullptr, nullptr, &cols, nullptr);
    cols /= m_frameWidth;

    SDL_Rect src = { (frameIndex % cols) * m_frameWidth, (frameIndex / cols) * m_frameHeight, m_frameWidth, m_frameHeight };

    SDL_Rect dst = { x, y, static_cast<int>(m_frameWidth * scale), static_cast<int>(m_frameHeight * scale) };

    renderer.drawSprite(m_texture, &src, &dst, angle, nullptr, SDL_FLIP_NONE);
}

void SpriteSheet::update(float dt) {
    if (!m_playing)
        return;

    m_elapsed += dt * m_speed;

    float frameDuration = 1.0f;
    while (m_elapsed >= frameDuration) {
        m_elapsed -= frameDuration;
        m_currentFrame++;

        if (m_currentFrame > m_endFrame) {
            if (m_loop) {
                m_currentFrame = m_startFrame;
            } else {
                m_currentFrame = m_endFrame;
                m_playing = false;
            }
        }
    }
}

void SpriteSheet::play(int startFrame, int endFrame, float speed, bool loop) {
    m_startFrame = startFrame;
    m_endFrame = endFrame;
    m_speed = speed;
    m_loop = loop;
    m_currentFrame = startFrame;
    m_elapsed = 0.0f;
    m_playing = true;
}

void SpriteSheet::stop() {
    m_playing = false;
}

void SpriteSheet::reset() {
    m_currentFrame = m_startFrame;
    m_elapsed = 0.0f;
}

} // namespace rr99
