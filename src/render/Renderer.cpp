#include "Renderer.h"

#include <iostream>

namespace rr99 {

Renderer::Renderer() = default;

Renderer::~Renderer() {
    if (m_font)
        TTF_CloseFont(m_font);
    if (m_fontSmall)
        TTF_CloseFont(m_fontSmall);
    if (m_fontLarge)
        TTF_CloseFont(m_fontLarge);
    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);
    if (m_window)
        SDL_DestroyWindow(m_window);
    TTF_Quit();
    IMG_Quit();
}

bool Renderer::initialize(const std::string& title, int width, int height, bool fullscreen) {
    m_width = width;
    m_height = height;

    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

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

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
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

void Renderer::resize(int w, int h) {
    m_width = w;
    m_height = h;
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

void Renderer::drawSprite(
    SDL_Texture* texture, const SDL_Rect* src, const SDL_Rect* dst, double angle, const SDL_Point* center,
    SDL_RendererFlip flip) {
    SDL_RenderCopyEx(m_renderer, texture, src, dst, angle, center, flip);
}

void Renderer::drawRect(const SDL_Rect* rect, SDL_Color color, bool filled) {
    if (!rect)
        return;
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    if (filled) {
        SDL_RenderFillRect(m_renderer, rect);
    } else {
        SDL_RenderDrawRect(m_renderer, rect);
    }
}

void Renderer::drawRect(const SDL_Rect& rect, SDL_Color color, bool filled) {
    drawRect(&rect, color, filled);
}

bool Renderer::loadFont(const std::string& path, int size) {
    if (m_font)
        TTF_CloseFont(m_font);
    m_font = TTF_OpenFont(path.c_str(), size);
    if (!m_font) {
        std::cerr << "Failed to load font: " << path << " - " << TTF_GetError() << std::endl;
        return false;
    }

    if (!m_fontSmall) {
        m_fontSmall = TTF_OpenFont(path.c_str(), 12);
    }
    if (!m_fontLarge) {
        m_fontLarge = TTF_OpenFont(path.c_str(), 24);
    }
    return true;
}

void Renderer::drawText(int x, int y, const std::string& text, SDL_Color color, int size) {
    TTF_Font* font = m_font;
    if (size <= 12 && m_fontSmall)
        font = m_fontSmall;
    else if (size >= 24 && m_fontLarge)
        font = m_fontLarge;

    if (!font)
        return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface)
        return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst = { x, y, surface->w, surface->h };
    SDL_RenderCopy(m_renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Renderer::drawTextCentered(int y, const std::string& text, SDL_Color color, int size) {
    TTF_Font* font = m_font;
    if (size <= 12 && m_fontSmall)
        font = m_fontSmall;
    else if (size >= 24 && m_fontLarge)
        font = m_fontLarge;

    if (!font)
        return;

    int textWidth;
    TTF_SizeText(font, text.c_str(), &textWidth, nullptr);
    int x = (m_width - textWidth) / 2;

    drawText(x, y, text, color, size);
}

int Renderer::textWidth(const std::string& text, int size) {
    TTF_Font* font = m_font;
    if (size <= 12 && m_fontSmall)
        font = m_fontSmall;
    else if (size >= 24 && m_fontLarge)
        font = m_fontLarge;

    if (!font)
        return 0;

    int w;
    TTF_SizeText(font, text.c_str(), &w, nullptr);
    return w;
}

} // namespace rr99
