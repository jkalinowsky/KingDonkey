#include "Highscores.h"

HighScore* loadHighScores(const char* fileName, int* numHighScores) {
    FILE* file = fopen(fileName, "r");

    if (file != NULL) {
        HighScore* highScores = NULL;
        int count = 0;
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            HighScore score;
            if (sscanf(buffer, "%s %d %f", score.nickname, &score.points, &score.time) == 3) {
                count++;

                highScores = (HighScore*)realloc(highScores, count * sizeof(HighScore));
                highScores[count - 1] = score;
            }
        }

        fclose(file);
        *numHighScores = count;
        return highScores;
    }
    else {
        printf("Failed to open the high scores file for reading.\n");
        *numHighScores = 0;
        return NULL;
    }
}

void bubbleSort(HighScore* highScores, int numHighScores) {
    for (int i = 0; i < numHighScores - 1; ++i) {
        for (int j = 0; j < numHighScores - i - 1; ++j) {
            if (highScores[j].points < highScores[j + 1].points) {
                HighScore temp = highScores[j];
                highScores[j] = highScores[j + 1];
                highScores[j + 1] = temp;
            }
        }
    }
}

void savePlayerHighscore(const char* nickname, int points, float time) {
    FILE* file = fopen("highscores.txt", "a");

    if (file != NULL) {
        fprintf(file, "%s %d %.2f\n", nickname, points, time);
        fclose(file);
    }
    else {
        printf("Failed to open the highscores file for writing.\n");
    }
}