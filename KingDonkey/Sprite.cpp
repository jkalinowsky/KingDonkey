#include "Sprite.h"

Sprite::Sprite(SDL_Renderer* renderer, const char* imagePath, float x, float y) {
    this->rect.x = x;
    this->rect.y = y;

    SDL_Surface* surface = SDL_LoadBMP(imagePath);
    if (!surface) {
        printf("Error loading texture: %s\n", SDL_GetError());
        return;
    }

    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!this->texture) {
        printf("Error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
    this->rect.w = surface->w;
    this->rect.h = surface->h;
    SDL_FreeSurface(surface);
}

Sprite::~Sprite() {
    SDL_DestroyTexture(texture);
}

void Sprite::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

SDL_Rect Sprite::getHitbox() {
    return rect;
}