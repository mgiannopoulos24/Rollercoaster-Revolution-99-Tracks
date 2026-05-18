#include "Sprite.h"

#include "Renderer.h"

namespace rr99 {

Sprite::~Sprite() {
    unload();
}

bool Sprite::load(Renderer& renderer, const std::string& path) {
    unload();
    m_texture = renderer.loadTexture(path);
    if (m_texture) {
        SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);
        return true;
    }
    return false;
}

void Sprite::unload() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
        m_width = 0;
        m_height = 0;
    }
}

void Sprite::draw(Renderer& renderer, int x, int y, float scale, double angle, SDL_Color tint, uint8_t alpha) const {
    if (!m_texture)
        return;

    SDL_Rect dst = { x, y, static_cast<int>(m_width * scale), static_cast<int>(m_height * scale) };

    SDL_SetTextureColorMod(m_texture, tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(m_texture, alpha);

    renderer.drawSprite(m_texture, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}

void Sprite::drawSection(Renderer& renderer, const SDL_Rect& srcRect, int x, int y, float scale, double angle) const {
    if (!m_texture)
        return;

    SDL_Rect dst = { x, y, static_cast<int>(srcRect.w * scale), static_cast<int>(srcRect.h * scale) };

    renderer.drawSprite(m_texture, &srcRect, &dst, angle, nullptr, SDL_FLIP_NONE);
}

} // namespace rr99
