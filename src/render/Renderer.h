#ifndef RR99_RENDER_RENDERER_H
#define RR99_RENDER_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>
#include <vector>

namespace rr99 {

class Sprite;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize(const std::string& title, int width, int height, bool fullscreen);
    void beginFrame();
    void endFrame();

    SDL_Renderer* getSDLRenderer() const { return m_renderer; }
    SDL_Window* getWindow() const { return m_window; }

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    void setLogicalSize(int w, int h);

    // Texture management
    SDL_Texture* loadTexture(const std::string& path);
    void unloadTexture(SDL_Texture* texture);

    // Sprite batching support (primitive)
    void drawSprite(SDL_Texture* texture, const SDL_Rect* src, const SDL_Rect* dst,
                    double angle = 0.0, const SDL_Point* center = nullptr,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);

    void drawRect(const SDL_Rect* rect, SDL_Color color, bool filled = false);

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    int m_width = 0;
    int m_height = 0;
    bool m_initialized = false;
};

} // namespace rr99

#endif // RR99_RENDER_RENDERER_H
