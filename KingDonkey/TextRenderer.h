#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "includes.h"

class TextRenderer
{
private:
    SDL_Renderer* renderer;
    SDL_Texture* charsetTexture;
    int charWidth, charHeight;
public:
    TextRenderer(SDL_Renderer* renderer);

    ~TextRenderer();

    void loadCharset();
    void drawString(int x, int y, float scale, const char* text, SDL_Color color);
    void drawMenu();
    void drawGameOver();
    void drawPlayerDied();
    void drawLevelCompleted();
    void drawLevelSelect();
    void drawBack();
    void drawStory();
    void drawGameEnd(int points, float time);
    void drawGameUI(const char* gameTimeText, const char* pointsText);
    void drawPoints(int x, int y, int points);
};

#endif // !TEXT_RENDERER_H