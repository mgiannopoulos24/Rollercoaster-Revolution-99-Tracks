#ifndef RR99_RENDER_SPRITE_H
#define RR99_RENDER_SPRITE_H

#include <SDL2/SDL.h>
#include <string>

namespace rr99 {

class Renderer;

class Sprite {
public:
    Sprite() = default;
    ~Sprite();

    bool load(Renderer& renderer, const std::string& path);
    void unload();

    void draw(
        Renderer& renderer, int x, int y, float scale = 1.0f, double angle = 0.0, SDL_Color tint = { 255, 255, 255, 255 },
        uint8_t alpha = 255) const;

    void drawSection(Renderer& renderer, const SDL_Rect& srcRect, int x, int y, float scale = 1.0f, double angle = 0.0) const;

    int getWidth() const {
        return m_width;
    }
    int getHeight() const {
        return m_height;
    }
    SDL_Texture* getTexture() const {
        return m_texture;
    }

private:
    SDL_Texture* m_texture = nullptr;
    int m_width = 0;
    int m_height = 0;
};

} // namespace rr99

#endif // RR99_RENDER_SPRITE_H
