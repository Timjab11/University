#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char string[51];
typedef struct Stonks {
    int day;
    int volume;
    char* name;
    float start;
    float end;
} Stonks;
int checkDigits(int x) {
    int numberOfDigits = 1;
    if (x == 0) {
        return 0;
    }
    while (x >= 10)
    {
        x /= 10;
        numberOfDigits++;
    }
    return numberOfDigits;
}
void printVolume(Stonks* share) {
    int numberLength = checkDigits(share->volume);
    int finalTextLength = numberLength + numberLength / 3 - 1;
    if (numberLength % 3 > 0) {
        finalTextLength++;
    }
    char* numberText = malloc(sizeof(char) * (finalTextLength + 1));
    numberText[finalTextLength + 1] == '\0';
    char* temp = malloc(sizeof(char) * (numberLength + 1));
    snprintf(temp, sizeof(char) * (numberLength + 1), "%d", share->volume);
    for (unsigned int i = numberLength + 1, count = 0, position = finalTextLength; i > 0; i--, count++, position--)
    {
        numberText[position] = temp[i - 1];
        if (count == 3) {
            count = 0;
            position--;
            numberText[position] = '_';
        }
    }
    printf(numberText);
    free(temp);
    free(numberText);
}
void writeOutStonks(Stonks*list , int size, char* stock) {
    for (size_t i = size; i > 0; i--)
    {
        if (strcmp(list[i-1].name,stock)==0) {
            printf("<tr>\n");
            printf("\t<td><b>%d</b></td>\n", list[i - 1].day);
            printf("\t<td><b>%s</b></td>\n", list[i - 1].name);
            printf("\t<td><b>%.2f</b></td>\n", list[i - 1].start);
            printf("\t<td><b>%.2f</b></td>\n", list[i - 1].end);
            printf("\t<td><b>%.2f</b></td>\n", list[i - 1].end - list[i - 1].start);
            printf("\t<td><b>");
            printVolume(&list[i - 1]);
            printf("</b></td>\n");
            printf("</tr>\n");
            continue;
        }
        printf("<tr>\n");
        printf("\t<td>%d</td>\n", list[i - 1].day);
        printf("\t<td>%s</td>\n", list[i - 1].name);
        printf("\t<td>%.2f</td>\n", list[i - 1].start);
        printf("\t<td>%.2f</td>\n", list[i - 1].end);
        printf("\t<td>%.2f</td>\n", list[i - 1].end - list[i - 1].start);
        printf("\t<td>");
        printVolume(&list[i - 1]);
        printf("</td>\n");
        printf("</tr>\n");
    }
}
void writeHighest(Stonks* share) {
    if (share == NULL) {
        printf("<div>\n");
        printf("Ticker AMC was not found\n");
        printf("</div>\n");
        return;
    }
    printf("<div>\n");
    printf("<h1>%s: highest volume</h1>\n",share->name);
    printf("<div>Day: %d</div>\n",share->day);
    printf("<div>Start price: %.2f</div>\n",share->start);
    printf("<div>End price: %.2f</div>\n",share->end);
    printf("<div>Volume: "); 
    printVolume(share);
    printf("</div>\n");
    printf("</div>\n");
}
void writeHTML(Stonks* share,Stonks* list, int size, char* stonks) {
    printf("<html>\n");
    printf("<body>\n");
    writeHighest(share);
    printf("<table>\n");
    printf("<thead>\n");
    printf("<tr><th>Day</th><th>Ticker</th><th>Start</th><th>End</th><th>Diff</th><th>Volume</th></tr>\n");
    printf("</thead>\n");
    printf("<tbody>\n");
    writeOutStonks(list, size, stonks);
    printf("</tbody>\n");
    printf("</table>\n");
    printf("</body>\n");
    printf("</html>\n");
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Wrong parameters\n");
        return 1;
    }

    char* stockNameInput = argv[1]; // název akcie
    int n = atoi(argv[2]);         // počet řádků
    struct Stonks* data = malloc(n * sizeof(struct Stonks));
    struct Stonks* saved = NULL;
    char input[100] = "";
    for (int i = 0; i < n; i++) {
        fgets(input, sizeof(input), stdin);
        string name = "";
        sscanf(input, "%d,%51[^,],%f,%f,%d",
            &data[i].day, name,
            &data[i].start, &data[i].end, &data[i].volume);
        data[i].name = malloc((strlen(name) + 1) * sizeof(char));
        strcpy(data[i].name,name);

        if (strcmp(data[i].name,stockNameInput) == 0) {
            //process it
            if (saved == NULL) {
                saved = &data[i];
                continue;
            }
            else if (data[i].volume > saved -> volume) {
                saved = &data[i];
            }
        }
    }
    writeHTML(saved,data,n,stockNameInput);
    //write out data

    for (size_t i = 0; i < n; i++)
    {
        free(data[i].name);
    }
    free(data);
    return 0;
}