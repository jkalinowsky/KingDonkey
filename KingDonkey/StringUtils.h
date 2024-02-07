#ifndef STRING_UTILS_H
#define STRING_UTILS_H

void appendString(char* dest, const char* src);
int calculateStringLength(const char* str);
void handleBackspace(char* nickname, int& cursorPosition);

#endif // STRING_UTILS_H