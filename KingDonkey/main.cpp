#include "Game.h"
#include "TextRenderer.h"

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

    // loading from file funcionality
    int numPlatforms = loadPlatformsNumber("./map.txt");
    int numLadders = loadLaddersNumber("./map.txt");
    int numTrophies = loadTrophiesNumber("./map.txt");

    Coordinates* platformCoordinates = new Coordinates[numPlatforms];
    Coordinates* ladderCoordinates = new Coordinates[numLadders];
    Coordinates* trophiesCoordinates = new Coordinates[numTrophies];

    loadPlatformCoordinates("./map.txt", platformCoordinates, numPlatforms);
    loadLadderCoordinates("./map.txt", ladderCoordinates, numLadders);
    loadTrophyCoordinates("./map.txt", trophiesCoordinates, numTrophies);

    Game game(renderer, 200, 600, platformCoordinates, numPlatforms, ladderCoordinates, numLadders, trophiesCoordinates, numTrophies);
    TextRenderer textRenderer(renderer);

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
                } 
                else if (event.key.keysym.sym == SDLK_SPACE && game.playerCompletedLevel()) {
                    game.increaseLevel();
                    restartGame = true;
                }
            }
        }
        int previousLives = game.getPlayerLives();

        // rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        if (!inMenu && game.playerCompletedLevel()) {
            gameState = LEVEL_COMPLETED;
            textRenderer.drawLevelCompleted();
        }

        if (!inMenu && gameState == PLAYING) {
            game.update(renderer);

            if (game.getPlayerLives() < previousLives) {
                previousLives = game.getPlayerLives();
                gameState = PLAYER_DEAD;
            }

            if (game.getPlayerLives() == 0) {
                gameState = GAME_OVER;
                inMenu = true;
            }
        }

        if (restartGame) {
            game.restart(renderer);
            if (gameState != PLAYER_DEAD && gameState != LEVEL_COMPLETED) {
                game.restartPlayerLives();
                game.restartPlayerPoints();
                game.restartLevel();
                game.restartGameTime();
            }
            gameState = PLAYING;
            restartGame = false;
            inMenu = false;
        }

        if (inMenu) {
            textRenderer.drawMenu();

            if (gameState == GAME_OVER) {
                textRenderer.drawGameOver();
            }
        }
        else {
            char livesText[20];
            char pointsText[20];
            snprintf(pointsText, sizeof(pointsText), "Points: %d", game.getPlayerPoints());
 
            textRenderer.drawGameUI(game.gameTimeText, pointsText);
            
            if (gameState == PLAYER_DEAD) {
                textRenderer.drawPlayerDied();
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