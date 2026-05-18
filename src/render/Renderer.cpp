#include "Renderer.h"
#include <iostream>

namespace rr99 {

Renderer::Renderer() = default;

Renderer::~Renderer() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    IMG_Quit();
}

bool Renderer::initialize(const std::string& title, int width, int height, bool fullscreen) {
    m_width = width;
    m_height = height;

    Uint32 flags = SDL_WINDOW_SHOWN;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    m_window = SDL_CreateWindow(title.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                width, height, flags);

    if (!m_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    m_renderer = SDL_CreateRenderer(m_window, -1, renderFlags);
    if (!m_renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }

    m_initialized = true;
    return true;
}

void Renderer::beginFrame() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::endFrame() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::setLogicalSize(int w, int h) {
    SDL_RenderSetLogicalSize(m_renderer, w, h);
}

SDL_Texture* Renderer::loadTexture(const std::string& path) {
    SDL_Texture* texture = IMG_LoadTexture(m_renderer, path.c_str());
    if (!texture) {
        std::cerr << "Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
    }
    return texture;
}

void Renderer::unloadTexture(SDL_Texture* texture) {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Renderer::drawSprite(SDL_Texture* texture, const SDL_Rect* src, const SDL_Rect* dst,
                          double angle, const SDL_Point* center, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(m_renderer, texture, src, dst, angle, center, flip);
}

void Renderer::drawRect(const SDL_Rect* rect, SDL_Color color, bool filled) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    if (filled) {
        SDL_RenderFillRect(m_renderer, rect);
    } else {
        SDL_RenderDrawRect(m_renderer, rect);
    }
}

} // namespace rr99
