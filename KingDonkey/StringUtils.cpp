#include "StringUtils.h"

void appendString(char* dest, const char* src) {
    int destLength = calculateStringLength(dest);
    int srcLength = calculateStringLength(src);

    for (int i = 0; i < srcLength; i++) {
        dest[destLength + i] = src[i];
    }

    dest[destLength + srcLength] = '\0';  // Null-terminate the string
}

int calculateStringLength(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}


void handleBackspace(char* nickname, int& cursorPosition) {
    if (cursorPosition > 0) {
        cursorPosition--;
        nickname[cursorPosition] = '\0';
    }
}