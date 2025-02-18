#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>


int main() {
    char amount[51] = "";
    fgets(amount, sizeof(amount), stdin);
    int n_lines = atoi(amount);
    char line[51] = "";
    char word[51] = "";
    for (int a = 0; a < n_lines; a++)
    {
        fgets(line, sizeof(line), stdin);
        bool wordStarted = false;
        bool hasUpper = false;
        int upperAmount = 0;
        int totalUpper = 0;
        int lowerAmount = 0;
        int totalLower = 0;
        int spaces = 0;
        int totalSpaces = 0;
        int length = (int)strlen(line);
        for (int i = 0; i < length; i++)
        {
            if (isalpha(line[i])) {
                if (wordStarted && line[i - 1] == ' ' && isalpha(line[i])) {
                    spaces++;
                    printf(" ");
                }
                wordStarted = true;
                if (isupper(line[i])) {
                    totalUpper += 1;
                    hasUpper = true;
                    word[(int)strlen(word) + 1] = '\0';
                    word[(int)strlen(word)] = line[i];
                }
                else {
                    totalLower += 1;
                    word[(int)strlen(word) + 1] = '\0';
                    word[(int)strlen(word)] = line[i];
                }
            }
            else if (line[i] == '\n') {
                if ((int)strlen(word) > 0) {
                    //IT IS A WORD
                    if (hasUpper == true) {
                        word[0] = (char)toupper(word[0]);
                        int placed = 1;
                        for (int i = 1; word[i] != '\0'; i++) {
                            if (word[placed] == '\0') {
                                word[i] = '\0';
                                break;
                            }
                            word[i] = (char)tolower(word[placed]);
                            while ((char)tolower(word[i]) == (char)tolower(word[placed]))
                            {
                                placed++;
                            }
                        }
                        printf("%s", word);
                        upperAmount += 1;
                        lowerAmount += ((int)strlen(word) - 1);
                        word[0] = '\0';
                    }
                    else {
                        int placed = 0;
                        for (int i = 0; word[i] != '\0'; i++) {
                            if (word[placed] == '\0') {
                                word[i] = '\0';
                                break;
                            }
                            word[i] = (char)toupper(word[placed]);
                            while ((char)toupper(word[i]) == (char)toupper(word[placed]))
                            {
                                placed++;
                            }
                        }
                        printf("%s", word);
                        upperAmount += (int)strlen(word);
                        word[0] = '\0';
                    }
                }
                hasUpper = false;
                printf("\n");
                printf("lowercase: %d -> %d\n", totalLower, lowerAmount);
                printf("uppercase: %d -> %d\n", totalUpper, upperAmount);
                printf("spaces: %d -> %d\n", totalSpaces, spaces);
            }
            else if (isspace(line[i])) {
                totalSpaces += 1;
                //do a reset nothing else
                if ((int)strlen(word) > 0) {
                    //IT IS A WORD
                    if (hasUpper == true) {
                        word[0] = (char)toupper(word[0]);
                        int placed = 1;
                        for (int i = 1; word[i] != '\0'; i++) {
                            if (word[placed] == '\0') {
                                word[i] = '\0';
                                break;
                            }
                            word[i] = (char)tolower(word[placed]);
                            while ((char)tolower(word[i]) == (char)tolower(word[placed]))
                            {
                                placed++;
                            }
                        }
                        printf("%s", word);
                        upperAmount += 1;
                        lowerAmount += ((int)strlen(word) - 1);
                        word[0] = '\0';
                    }
                    else {
                        int placed = 0;
                        for (int i = 0; word[i] != '\0'; i++) {
                            if (word[placed] == '\0') {
                                word[i] = '\0';
                                break;
                            }
                            word[i] = (char)toupper(word[placed]);
                            while ((char)toupper(word[i]) == (char)toupper(word[placed]))
                            {
                                placed++;
                            }
                        }
                        printf("%s", word);
                        upperAmount += (int)strlen(word);
                        word[0] = '\0';
                    }
                }
                hasUpper = false;
            }
        }
        if (n_lines-1 > a) {
            printf("\n");
        }
    }
    return 0;
}