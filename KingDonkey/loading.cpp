#include "loading.h"

int loadPlatformsNumber(const char* filePath) {
    FILE* file = fopen(filePath, "r");

    if (!file) {
        perror("Error opening file");
        printf("File path: %s\n", filePath);
        return 0;
    }

    int numPlatforms;
    if (fscanf(file, "%d", &numPlatforms) != 1) {
        perror("Error reading the number of platforms");
        fclose(file);
        return 0;
    }

    fclose(file);
    return numPlatforms;
}

void loadPlatformCoordinates(const char* filePath, Coordinates platformCoordinates[], int numPlatforms) {
    FILE* file = fopen(filePath, "r");

    if (!file) {
        perror("Error opening file");
        printf("File path: %s\n", filePath);
        return;
    }

    // Skip the first line containing the number of platforms
    int num;
    if (fscanf(file, "%d", &num) != 1) {
        perror("Error reading the number of platforms");
        fclose(file);
        return;
    }

    if (num <= 0) {
        printf("Invalid number of platforms: %d\n", num);
        fclose(file);
        return;
    }

    // Read platform coordinates
    for (int i = 0; i < numPlatforms; ++i) {
        int x, y;
        if (fscanf(file, "%d %d", &x, &y) == 2) {
            platformCoordinates[i].x = x;
            platformCoordinates[i].y = y;
        }
        else {
            perror("Error reading platform coordinates");
            break;
        }
    }

    fclose(file);
}

int loadLaddersNumber(const char* filePath) {
    FILE* file = fopen(filePath, "r");

    if (!file) {
        perror("Error opening file");
        printf("File path: %s\n", filePath);
        return 0;
    }

    int numPlatforms;
    // Read the number of platforms
    if (fscanf(file, "%d", &numPlatforms) != 1) {
        perror("Error reading the number of platforms");
        fclose(file);
        return 0;
    }

    int numLadders = 0;

    // Skip lines * numPlatforms to reach the line with the number of ladders
    for (int i = 0; i < numPlatforms; ++i) {
        int x, y;
        if (fscanf(file, "%d %d", &x, &y) != 2) {
            perror("Error reading platform coordinates");
            fclose(file);
            return 0;
        }
    }

    // Read the number of ladders
    if (fscanf(file, "%d", &numLadders) != 1) {
        perror("Error reading the number of ladders");
        fclose(file);
        return 0;
    }

    fclose(file);
    return numLadders;
}


void loadLadderCoordinates(const char* filePath, Coordinates ladderCoordinates[], int numLadders) {
    FILE* file = fopen(filePath, "r");

    if (!file) {
        perror("Error opening file");
        printf("File path: %s\n", filePath);
        return;
    }

    // Skip the first lines containing the number of platforms
    int num;
    if (fscanf(file, "%d", &num) != 1) {
        perror("Error reading the number of platforms");
        fclose(file);
        return;
    }

    if (num <= 0) {
        printf("Invalid number of platforms: %d\n", num);
        fclose(file);
        return;
    }

    // Skip the platform coordinates
    for (int i = 0; i < num; ++i) {
        int x, y;
        if (fscanf(file, "%d %d", &x, &y) != 2) {
            perror("Error reading platform coordinates");
            fclose(file);
            return;
        }
    }

    if (fscanf(file, "%d", &num) != 1) {
        perror("Error reading line before ladder coordinates");
        fclose(file);
        return;
    }

    // Read ladder coordinates
    for (int i = 0; i < numLadders; ++i) {
        int x, y;
        if (fscanf(file, "%d %d", &x, &y) == 2) {
            ladderCoordinates[i].x = x;
            ladderCoordinates[i].y = y;
        }
        else {
            perror("Error reading ladder coordinates");
            break;
        }
    }

    fclose(file);
}

int loadTrophiesNumber(const char* filePath) {
    FILE* file = fopen(filePath, "r");

    if (!file) {
        perror("Error opening file");
        printf("File path: %s\n", filePath);
        return 0;
    }

    int numPlatforms;
    // Skip the number of platforms
    if (fscanf(file, "%d", &numPlatforms) != 1) {
        perror("Error reading the number of platforms");
        fclose(file);
        return 0;
    }

    int numLadders;
    // Skip the number of ladders
    if (fscanf(file, "%d", &numLadders) != 1) {
        perror("Error reading the number of ladders");
        fclose(file);
        return 0;
    }

    int numTrophies;
    // Read the number of trophies
    if (fscanf(file, "%d", &numTrophies) != 1) {
        perror("Error reading the number of trophies");
        fclose(file);
        return 0;
    }

    fclose(file);
    return numTrophies;
}

void loadTrophyCoordinates(const char* filePath, Coordinates trophyCoordinates[], int numTrophies) {
    FILE* file = fopen(filePath, "r");

    if (!file) {
        perror("Error opening file");
        printf("File path: %s\n", filePath);
        return;
    }

    // Skip the first lines containing the number of platforms and ladders
    int num;
    if (fscanf(file, "%d", &num) != 1) {
        perror("Error reading the number of platforms");
        fclose(file);
        return;
    }

    if (num <= 0) {
        printf("Invalid number of platforms: %d\n", num);
        fclose(file);
        return;
    }

    for (int i = 0; i < num; ++i) {
        int x, y;
        if (fscanf(file, "%d %d", &x, &y) != 2) {
            perror("Error reading platform coordinates");
            fclose(file);
            return;
        }
    }

    if (fscanf(file, "%d", &num) != 1) {
        perror("Error reading the number of ladders");
        fclose(file);
        return;
    }

    for (int i = 0; i < num; ++i) {
        int x, y;
        if (fscanf(file, "%d %d", &x, &y) != 2) {
            perror("Error reading ladder coordinates");
            fclose(file);
            return;
        }
    }

    if (fscanf(file, "%d", &num) != 1) {
        perror("Error reading line before trophy coordinates");
        fclose(file);
        return;
    }

    // Read trophy coordinates
    for (int i = 0; i < numTrophies; ++i) {
        int x, y;
        if (fscanf(file, "%d %d", &x, &y) == 2) {
            trophyCoordinates[i].x = x;
            trophyCoordinates[i].y = y;
        }
        else {
            perror("Error reading trophy coordinates");
            break;
        }
    }

    fclose(file);
}