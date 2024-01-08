#ifndef SPRITE_H
#define SPRITE_H

#include "includes.h"

class Sprite {
public:
    SDL_Rect rect;
    SDL_Texture* texture;

    Sprite();
    Sprite(SDL_Renderer* renderer, const char* imagePath, float x, float y);
    ~Sprite();

    void render(SDL_Renderer* renderer);
};

#endif // !SPRITE_H