#include "Game.h"

enum GameState {
    PLAYING,
    PLAYER_DEAD,
    GAME_OVER
};

void DrawString(SDL_Renderer* renderer, int x, int y, int font, const char* text, SDL_Texture* charsetTexture) {
    int px, py, c;
    SDL_Rect src = {
        0,
        0,
        8,
        8,
    };

    SDL_Rect dest = {
        x,
        y,
        8 * font,
        8 * font,
    };

    for (int i = 0; text[i] != '\0'; i++)
    {
        c = text[i] & 255;
        src.x = (c % 16) * 8;
        src.y = (c / 16) * 8;
        dest.x = x + i * 8 * font;

        SDL_RenderCopy(renderer, charsetTexture, &src, &dest);
    }
}

void DrawMenu(SDL_Renderer* renderer, SDL_Texture* charsetTexture, int fontSize) {
    int x = 200;
    int y = 200;

    DrawString(renderer, x, y, fontSize, "Press 'N' to start a new game", charsetTexture);
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    // window and renderer creation and settings
    SDL_Window* window;
    SDL_Renderer* renderer;
    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(window, "King Donkey");

    SDL_Surface* charset = SDL_LoadBMP("./graphics/cs8x8.bmp");
    
    SDL_Texture* charsetTexture = SDL_CreateTextureFromSurface(renderer, charset);
    SDL_FreeSurface(charset);

    // loading from file funcionality
    int numPlatforms = loadPlatformsNumber("./map.txt");
    int numLadders = loadLaddersNumber("./map.txt");

    Coordinates* platformCoordinates = new Coordinates[numPlatforms];
    Coordinates* ladderCoordinates = new Coordinates[numLadders];

    loadPlatformCoordinates("./map.txt", platformCoordinates, numPlatforms);
    loadLadderCoordinates("./map.txt", ladderCoordinates, numLadders);

    Game game(renderer, 200, 600, platformCoordinates, numPlatforms, ladderCoordinates, numLadders);


    // main loop and variables
    SDL_Event event;
    bool quitProgram = false;
    bool restartGame = false;
    bool inMenu = true;

    GameState gameState = PLAYING;

    while (!quitProgram) {
        // event handling
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitProgram = true;
                }
                else if (event.key.keysym.sym == SDLK_n) {
                    restartGame = true;
                    gameState = PLAYING;
                }
                else if (event.key.keysym.sym == SDLK_SPACE && gameState == PLAYER_DEAD) {
                    restartGame = true;
                    gameState = PLAYING;
                }
            }
        }
        int previousLives = game.getPlayerLives();

        if (!inMenu && gameState == PLAYING) {
            game.update(renderer);

            if (game.getPlayerLives() < previousLives) {
                previousLives = game.getPlayerLives();
                gameState = PLAYER_DEAD;
            }

            if (game.getPlayerLives() == 0) {
                gameState = GAME_OVER;
                inMenu = true;
                game.restartPlayerLives();
            }
        }

        if (restartGame) {
            game.restart(renderer);
            restartGame = false;
            inMenu = false;
        }

        // rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        if (inMenu) {
            DrawMenu(renderer, charsetTexture, 4);

            if (gameState == GAME_OVER) {
                DrawString(renderer, 100, 150, 2, "You died, try again", charsetTexture);
            }
        }
        else {
            char livesText[20];
            char pointsText[20];
            snprintf(livesText, sizeof(livesText), "Lives: %d", game.getPlayerLives());
            snprintf(pointsText, sizeof(pointsText), "Points: %d", game.getPlayerPoints());
 
            DrawString(renderer, 10, 10, 1, game.gameTimeText, charsetTexture);
            DrawString(renderer, 10, 30, 1, livesText, charsetTexture);
            DrawString(renderer, 1120, 10, 1, pointsText, charsetTexture);
            
            if (gameState == PLAYER_DEAD) {
                DrawString(renderer, 496, 30, 1, "If you want to continue, press SPACE", charsetTexture);
            }
            
            game.render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    // cleaning up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}