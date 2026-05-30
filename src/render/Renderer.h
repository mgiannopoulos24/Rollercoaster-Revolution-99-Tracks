#ifndef RR99_RENDER_RENDERER_H
#define RR99_RENDER_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
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

    SDL_Renderer* getSDLRenderer() const {
        return m_renderer;
    }
    SDL_Window* getWindow() const {
        return m_window;
    }

    int getWidth() const {
        return m_width;
    }
    int getHeight() const {
        return m_height;
    }

    void setLogicalSize(int w, int h);
    void resize(int w, int h);

    // Texture management
    SDL_Texture* loadTexture(const std::string& path);
    void unloadTexture(SDL_Texture* texture);

    // Sprite batching support (primitive)
    void drawSprite(
        SDL_Texture* texture, const SDL_Rect* src, const SDL_Rect* dst, double angle = 0.0, const SDL_Point* center = nullptr,
        SDL_RendererFlip flip = SDL_FLIP_NONE);

    void drawRect(const SDL_Rect* rect, SDL_Color color, bool filled = false);
    void drawRect(const SDL_Rect& rect, SDL_Color color, bool filled = false);

    // Text rendering (SDL_ttf)
    bool loadFont(const std::string& path, int size = 16);
    void drawText(int x, int y, const std::string& text, SDL_Color color, int size = 16);
    void drawTextCentered(int y, const std::string& text, SDL_Color color, int size = 16);
    int textWidth(const std::string& text, int size = 16);

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    TTF_Font* m_font = nullptr;
    TTF_Font* m_fontSmall = nullptr;
    TTF_Font* m_fontLarge = nullptr;
    int m_width = 0;
    int m_height = 0;
    bool m_initialized = false;
};

} // namespace rr99

#endif // RR99_RENDER_RENDERER_H
