#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "includes.h"

class TextRenderer
{
private:
    SDL_Renderer* renderer;
    SDL_Texture* charsetTexture;

public:
    TextRenderer(SDL_Renderer* renderer);
    ~TextRenderer();

    void loadCharset();
    void drawString(int x, int y, int font, const char* text);
    void drawMenu();
    void drawGameOver();
    void drawPlayerDied();
    void drawLevelCompleted();
    void drawGameUI(const char* gameTimeText, const char* pointsText);
    void drawPoints(int x, int y, int points);
};

#endif // !TEXT_RENDERER_H