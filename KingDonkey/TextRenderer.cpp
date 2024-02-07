#include "TextRenderer.h"

SDL_Color whiteColor = { 255, 255, 255 };
SDL_Color blueColor = { 0, 0, 255 };
SDL_Color greenColor = { 0, 255, 0 };
SDL_Color redColor = { 255, 0, 0 };

TextRenderer::TextRenderer(SDL_Renderer* renderer)
    : renderer(renderer), charsetTexture(nullptr), charWidth(8), charHeight(8) {
    loadCharset();
}

TextRenderer::~TextRenderer() {
    if (charsetTexture != nullptr) {
        SDL_DestroyTexture(charsetTexture);
    }
}

void TextRenderer::loadCharset() {
    SDL_Surface* charset = SDL_LoadBMP("./graphics/cs8x8.bmp");

    if (charset != nullptr) {
        charsetTexture = SDL_CreateTextureFromSurface(renderer, charset);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        SDL_FreeSurface(charset);
    }
    else {
        printf("Failed to load charset texture!\n");
    }
}

void TextRenderer::drawString(int x, int y, float scale, const char* text, SDL_Color color) {
    int c;
    SDL_Rect src = { 0, 0, charWidth, charHeight };
    SDL_Rect dest = { x, y, charWidth * scale, charHeight * scale };

    SDL_SetTextureColorMod(charsetTexture, color.r, color.g, color.b);

    for (int i = 0; text[i] != '\0'; i++) {
        c = text[i] & 255;
        src.x = (c % 16) * charWidth;
        src.y = (c / 16) * charHeight;
        dest.x = x + i * charWidth * scale;

        SDL_RenderCopy(renderer, charsetTexture, &src, &dest);
    }
    SDL_SetTextureColorMod(charsetTexture, 255, 255, 255);
}

void TextRenderer::drawMenu() {
    drawString(160, 120, 4.0, "Press 'N' to start a new game.", greenColor);
    drawString(128, 240, 4.0, "Press 'H' to see the highscores.", whiteColor);
    drawString(192, 360, 4.0, "Press 'L' to select a level.", whiteColor);
    drawString(256, 480, 4.0, "Press 'S' to read story.", whiteColor);
    drawString(320, 600, 4.0, "Press 'ESC' to quit.", redColor);
}

void TextRenderer::drawGameOver() {
    drawString(304, 250, 2.0, "You died. If you want to save your result:", redColor);
    drawString(432, 275, 2.0, "press 'Y', else press 'N'.", redColor);
}

void TextRenderer::drawPlayerDied() {
    drawString(336, 30, 2.0, "If you want to continue, press 'SPACE'", redColor);
}

void TextRenderer::drawLevelCompleted() {
    drawString(296, 30, 2.0, "Level completed! press 'SPACE' to continue.", blueColor);
}

void TextRenderer::drawLevelSelect() {
    drawString(432, 100, 4.0, "Select level:", blueColor);
    drawString(304, 300, 3.0, "Press '1' to select level 1.", whiteColor);
	drawString(304, 410, 3.0, "Press '2' to select level 2.", whiteColor);
	drawString(304, 520, 3.0, "Press '3' to select level 3.", whiteColor);
}

void TextRenderer::drawBack() {
    drawString(10, 700, 2.0, "Press 'X' to go back.", greenColor);
}

void TextRenderer::drawStory() {
    drawString(124, 100, 1.5, "In one of a million different realities, Mario became a villain, constantly tormenting", whiteColor);
    drawString(148, 140, 1.5, "Donkey Kong. However, on a particular day, Mario was in an exceptionally bad mood,", whiteColor);
    drawString(172, 180, 1.5, "and it happened to be Donkey's birthday. As Donkey Kong peacefully planned his", whiteColor);
    drawString(124, 220, 1.5, "birthday party, enjoying the birthday cake with guests and about to open presents, the", whiteColor);
    drawString(214, 260, 1.5, "malicious Mario showed up, determined to ruin the birthday celebration.", whiteColor);
    drawString(344, 450, 2.0, "You are the one embodying this Mario!", redColor);
}

void TextRenderer::drawGameEnd(int points, float time, GameState gameState) {
    char pointsText[20], timeText[20];
    sprintf(pointsText, "%d", points);
    sprintf(timeText, "%.2f", time);
    if (gameState == GAME_COMPLETED)
        drawString(225, 50, 4.0, "You won! Congratulations!", greenColor);
    else 
        drawString(448, 50, 4.0, "You lose! :(", redColor);
    drawString(412, 100, 2.0, "Your points:", whiteColor);
    drawString(470, 120, 2.0, pointsText, whiteColor);
    drawString(768, 100, 2.0, "Your time:", whiteColor);
    drawString(800, 120, 2.0, timeText, whiteColor);
    drawString(304, 300, 2.0, "Type your nickname to save your highscore:", whiteColor);
}

void TextRenderer::drawNickname(const char* nickname, int cursorPosition) {
    drawString(304, 350, 2.0, nickname, whiteColor);

    int cursorX = 304 + calculateStringLength(nickname) * charWidth * 2.0;
    int cursorY = 350;
    drawString(cursorX, cursorY, 2.0, "|", whiteColor);
}

void TextRenderer::drawHighscores(HighScore* highScores, int numHighScores, int totalPages, int scoresPerPage, int currentPage) {
    for (int page = 1; page <= totalPages; ++page) {
        if (page == currentPage) {
            int startIdx = (page - 1) * scoresPerPage;
            int endIdx = (startIdx + scoresPerPage < numHighScores) ? startIdx + scoresPerPage : numHighScores;

            for (int i = startIdx; i < endIdx; ++i) {
                char rankText[5];
                char pointsText[20];
                char timeText[30];


                sprintf(rankText, "%d.", i + 1);
                sprintf(pointsText, "Points: %d", highScores[i].points);
                sprintf(timeText, "Time: %.2f", highScores[i].time);

                drawString(300, 100 + (i - startIdx) * 50, 2.0, rankText, whiteColor);
                drawString(350, 100 + (i - startIdx) * 50, 2.0, highScores[i].nickname, whiteColor);
                drawString(650, 100 + (i - startIdx) * 50, 2.0, pointsText, whiteColor);
                drawString(850, 100 + (i - startIdx) * 50, 2.0, timeText, whiteColor);
            }

            char pageText[20];
            sprintf(pageText, "Page %d/%d", page, totalPages);
            drawString(150, 600, 2.0, pageText, whiteColor);
        }
    }
}

void TextRenderer::drawGameUI(const char* gameTimeText, const char* pointsText) {
    drawString(10, 10, 1, gameTimeText, whiteColor);
    drawString(1120, 60, 1, pointsText, whiteColor);
}

void TextRenderer::drawPoints(int x, int y, int points) {
	char pointsText[10];
	sprintf(pointsText, "%d", points);
	drawString(x, y, 1, pointsText, greenColor);
}