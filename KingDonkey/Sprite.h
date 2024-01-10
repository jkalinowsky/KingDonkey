#ifndef SPRITE_H
#define SPRITE_H

#include "includes.h"

class Sprite {
protected:
    SDL_Rect rect;
    SDL_Texture* texture;
public:

    Sprite(SDL_Renderer* renderer, const char* imagePath, float x, float y);
    ~Sprite();

    void render(SDL_Renderer* renderer);

    SDL_Rect getHitbox();
};

#endif // !SPRITE_H