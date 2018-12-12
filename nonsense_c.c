#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


char **players = 0;
int playersCount = 0;

char **consistance = 0;
int consistanceCount = 0;

char **history = 0;
int historyCount = 0;


void saveHistory();

void readHistory();

void saveConsistance();

void readConsistance();

void addPart(char *);

void addHistory(char *, char * = 0);

void viewHistory();

void mainMenu();

void play();

void setPlayers();

void setConsistance(bool = true);

void consistanceSetDefaults();

bool isInputCorrect(char*, int);

void pressEnterToContinue();

char *getSentenceWithoutPlayers(char *);

int main() 
{
    readHistory();
    readConsistance();

    mainMenu();

    for(int i = 0; i < playersCount; i++)
    {
        free(players[i]);
    }
    free(players);

    for(int i = 0; i < consistanceCount; i++)
    {
        free(consistance[i]);
    }
    free(consistance);

    for(int i = 0; i < historyCount; i++)
    {
        free(history[i]);
    }
    free(history);

    return 0;
}

void mainMenu() 
{
    while(true)
    {
        system("clear");
        printf("Nonsense Game v1.0\n"
               "Main menu\n"
               "1. Set the players\n"
               "2. Set the consistance of the sentence\n"
               "3. Play\n"
               "4. View the history\n"
               "5. Exit\n"
               ">> ");
        char string[10];
        scanf ("%[^\n]%*c", string);
        if(!isInputCorrect(string, 5)) {
            puts("Invalid value\n");
            pressEnterToContinue();
            continue;
        }

        int number = atoi(string);

        switch(number)
        {
        case 1: setPlayers(); break;
        case 2: setConsistance(); break;
        case 3: play(); break;
        case 4: viewHistory(); break;
        case 5: return; break;
        }
    }

}

void play()
{
    int sentenceSize = 1;
    int playerIndex = 0;
    int consistanceIndex = 0;
    char *sentence = (char *) calloc(1, sizeof(char));
    sentence[0] = '\0';
    while(1)
    {
        system("clear");
        if(playersCount == 0)
        {
            printf("There must be at least one player\n");
            pressEnterToContinue();
            setPlayers();
            continue;
        }
        if(consistanceCount == 0) setConsistance(false);
        char string[100];
        printf("Enter 1 to go back to the main main\n"
               "Enter 0 to finish a game\n"
               "%s (%s) >> ", consistance[consistanceIndex],
               players[playerIndex]);
        scanf ("%[^\n]%*c", string);
        if ((string[0] == '0' && string[1] == '\0') ||
                consistanceIndex >= consistanceCount - 1)
        {
            consistanceIndex = 0;
            char *sentenceToPrint = getSentenceWithoutPlayers(sentence);
            printf(sentenceToPrint);
            free(sentenceToPrint);
            printf("\n");
            addHistory(sentence);
            saveHistory();
            free(sentence);
            sentence = (char *) calloc(1, sizeof(char));
            sentence[0] = '\0';
            sentenceSize = 1;
            pressEnterToContinue();
        }
        else if (string[0] == '1' && string[1] == '\0') break;
        else
        {
            consistanceIndex++;
            int newSize = sentenceSize + strlen(string) + 2 + strlen(players[playerIndex]) + 2;
            sentenceSize = newSize;
            char *newSentence = (char*) calloc(newSize, sizeof(char));
            strcpy(newSentence, sentence);
            strcat(newSentence, string);
            strcat(newSentence, " [");
            strcat(newSentence, players[playerIndex]);
            strcat(newSentence, "] ");
            free(sentence);
            sentence = (char*) calloc(newSize, sizeof(char));
            strcpy(sentence, newSentence);
            free(newSentence);
            if(playerIndex >= playersCount - 1) playerIndex = 0;
            else playerIndex++;
        }
    }
    free(sentence);
    system("clear");
}

void setPlayers()
{
    while(true)
    {
        system("clear");
        printf("Set the players\n"
               "1. Add a new player\n"
               "2. Back\n");
        if(playersCount != 0)
        {
            for(int i = 0; i < playersCount; i++)
            {
                printf("%d. Edit %s\n", i + 3, players[i]);
            }
        }
        printf(">> ");
        char string[10];
        scanf ("%[^\n]%*c", string);
        if(!isInputCorrect(string, playersCount + 2)) {
            puts("Invalid value\n");
            pressEnterToContinue();
            continue;
        }

        int number = atoi(string);

        switch(number)
        {
        case 1: {
            system("clear");
            printf("Enter a name\nEnter 0 to cancel>> ");
            char *temp = (char*) calloc(20, sizeof(char));
            scanf ("%[^\n]%*c", temp);
            if(temp[0] == '0' && temp[1] == '\0') {
                break;
                free(temp);
            }
            char **newArray = (char**) calloc(++playersCount, sizeof(char*));
            for(int i = 0; i < playersCount - 1; i++)
            {
                newArray[i] = players[i];
            }
            newArray[playersCount - 1] = temp;
            free(players);
            players = newArray;
            break;
        }
        case 2: return;
        }

        for(int i = 0; i < playersCount; i++)
        {
            if(number == i + 1 + 2)
            {
                while(true)
                {
                    system("clear");
                    printf("Editing \"%s\"\n"
                           "1. Delete\n"
                           "2. Enter a new name\n"
                           "3. Cancel\n"
                           ">> ", players[i]);

                    scanf ("%[^\n]%*c", string);
                    if(!isInputCorrect(string, 3)) {
                        puts("Invalid value\n");
                        pressEnterToContinue();
                        continue;
                    }

                    int num = atoi(string);

                    switch(num)
                    {
                    case 1:
                    {
                        char **newArray =
                                (char**) malloc(sizeof(char*) * --playersCount);
                        for(int j = 0; j < playersCount + 1; j++)
                        {
                            if(j == i) continue;
                            else newArray[j] = players[j];
                        }
                        free(players);
                        players = newArray;
                        break;
                    }
                    case 2:
                    {
                        system("clear");
                        printf("Enter a new name\nPress 0 to cancel\n>> ");
                        char *tmp = (char*) malloc(sizeof(char) * 100);
                        scanf ("%[^\n]%*c", tmp);
                        if(tmp[0] == '0' && tmp[1] == '\0') break;
                        free(players[i]);
                        players[i] = tmp;
                    }
                    case 3: break;
                    }
                    break;
                }
            }
        }
    }
}

void setConsistance(bool ranFromMenu)
{
    if(consistanceCount == 0)
    {
        consistanceSetDefaults();
        if(!ranFromMenu) return;
    }

    while(true)
    {
        system("clear");
        printf("Editing the consistance of sentences\n"
               "1. Add a new part\n"
               "2. Back\n"
               "3. Set defaults\n");
        if(consistanceCount != 0)
        {
            for(int i = 0; i < consistanceCount; i++)
            {
                printf("%d. Edit %s\n", i + 4, consistance[i]);
            }
        }
        printf(">> ");
        char string[10];
        scanf ("%[^\n]%*c", string);
        if(!isInputCorrect(string, consistanceCount + 3)) {
            puts("Invalid value\n");
            pressEnterToContinue();
            continue;
        }

        int number = atoi(string);

        switch(number)
        {
        case 1: {
            system("clear");
            printf("Enter a part\nEnter 0 to cancel>> ");
            char *temp = (char*) calloc(20, sizeof(char));
            scanf ("%[^\n]%*c", temp);
            if(temp[0] == '0' && temp[1] == '\0') {
                break;
                free(temp);
            }
            addPart(temp);
            saveConsistance();
            break;
        }
        case 2: return; break;
        case 3: consistanceSetDefaults(); break;
        }

        for(int i = 0; i < consistanceCount; i++)
        {
            if(number == i + 1 + 3)
            {
                while(true)
                {
                    system("clear");
                    printf("Editing \"%s\"\n"
                           "1. Delete\n"
                           "2. Enter a new name\n"
                           "3. Cancel\n"
                           ">> ", consistance[i]);

                    scanf ("%[^\n]%*c", string);
                    if(!isInputCorrect(string, 3)) {
                        puts("Invalid value\n");
                        pressEnterToContinue();
                        continue;
                    }

                    int num = atoi(string);

                    switch(num)
                    {
                    case 1:
                    {
                        char **newArray =
                                (char**) malloc(sizeof(char*) *
                                                --consistanceCount);
                        for(int j = 0; j < consistanceCount + 1; j++)
                        {
                            if(j == i) continue;
                            else newArray[j] = consistance[j];
                        }
                        free(consistance);
                        consistance = newArray;
                        break;
                    }
                    case 2:
                    {
                        system("clear");
                        printf("Enter a new name\nPress 0 to cancel\n>> ");
                        char *tmp = (char*) malloc(sizeof(char) * 100);
                        scanf ("%[^\n]%*c", tmp);
                        if(tmp[0] == '0' && tmp[1] == '\0') break;
                        free(consistance[i]);
                        consistance[i] = tmp;
                    }
                    case 3: break;
                    }
                    break;
                }
            }
        }
    }
}

void consistanceSetDefaults()
{
    char *defaultArray[] = {"What is like?", "Who\\what?",
                            "Does\\did\\will do what?", "How?",
                            "Along with whom?", "When?", "Where?"};
    int defaultLength = 7;
    consistance = (char**) calloc(defaultLength, sizeof(char*));
    for(int i = 0; i < defaultLength; i++)
    {
        consistance[i] =
                (char*) malloc(strlen(defaultArray[i]) * sizeof(char));
        strcpy(consistance[i], defaultArray[i]);
    }
    consistanceCount = defaultLength;
    saveConsistance();
}

bool isInputCorrect(char *string, int itemsCount)
{

    return !(string[0] == '\n' || (strlen(string) > 2) ||
            strlen(string) == 0 ||
            !(isdigit(string[0])) ||
            !(string[1] == '\0' || isdigit(string[1])) ||
            atoi(string) > itemsCount);
}

void pressEnterToContinue()
{
    printf("Press enter to continue\n");
    getchar();
}

void addHistory(char *sentence, char *itemTime)
{
    bool emptyItemTime = !itemTime;
    if(emptyItemTime)
    {
        time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        itemTime = asctime (timeinfo);
    }
    else
    {
        char *newItemTime =
                (char*) calloc(strlen(itemTime) + 1, sizeof(char));
        strcpy(newItemTime, itemTime);
        itemTime = newItemTime;
    }

    char *newItem = (char*) calloc(strlen(sentence) + 1
                                   + strlen(itemTime) + 2, sizeof(char));
    strcpy(newItem, itemTime);
    if(!emptyItemTime) strcat(newItem, "\n");
    strcat(newItem, sentence);
    strcat(newItem, "\n");

    char **newArray = (char**) malloc(++historyCount * sizeof(char*));
    for(int i = 0; i < historyCount - 1; i++)
    {
        newArray[i] = history[i];
    }
    free(history);
    newArray[historyCount - 1] = newItem;
    history = newArray;
}

void viewHistory()
{
    system("clear");
    printf("Viewing the history\n");
    for(int i = 0; i < historyCount; i++)
    {
        printf(history[i]);
        printf("\n");
    }
    pressEnterToContinue();
}

char *getSentenceWithoutPlayers(char *sentence)
{
    int begin = 0;
    int end = 0;
    int sentenceLength = strlen(sentence);
    char *retSentence = (char*) calloc(strlen(sentence + 1), sizeof(char));
    strcpy(retSentence, sentence);
    while(true)
    {
        char *_begin = strchr(retSentence, '[');
        char *_end = strchr(retSentence, ']');
        if(_begin == 0 || _end == 0) break;

        begin = _begin - retSentence;
        end = _end - retSentence;
        int playerLength = end - begin + 2;
        char *newSentence = (char *) calloc(sentenceLength - playerLength,
                                            sizeof(char));
        bool cuting = false;
        for(int i = 0, j = 0; i < sentenceLength + 1; i++)
        {
            if(i == begin) cuting = true;
            else if(i == end)
            {
                cuting = false;
                i++;
                continue;
            }
            if(!cuting)
            {
                newSentence[j++] = retSentence[i];
            }
        }
        free(retSentence);
        retSentence = newSentence;
    }
    return retSentence;
}

void readHistory()
{
    FILE *histFile;
    histFile = fopen("history.txt", "r");

    if(histFile == 0)
    {
        printf("Warning: Error reading history from the file!\n");
        pressEnterToContinue();
        return;
    }

    char itemTime[50];
    char itemSentence[100];
    while(true)
    {
        int resTime = fscanf(histFile, "%[^\n]%*c", itemTime);
        int resSentence = fscanf(histFile, "%[^\n]%*c", itemSentence);
        if(resTime != 1 || resSentence != 1) break;
        if(feof(histFile)) break;
        addHistory(itemSentence, itemTime);
        itemTime[0] = '\0';
        itemSentence[0] = '\0';
    }

    fclose(histFile);
}

void saveHistory()
{
    FILE *histFile;
    histFile = fopen("history.txt", "w");

    if(histFile == 0)
    {
        printf("Error saving history to the file: can't open the file\n");
        pressEnterToContinue();
        return;
    }

    for(int i = 0; i < historyCount; i++)
    {
        if(fprintf(histFile, history[i]) < 0)
        {
            printf("Error saving history to the file: "
                   "can't write some items\n");
            pressEnterToContinue();
            return;
        }
    }

    fclose(histFile);
}

void readConsistance()
{
    FILE *consFile;
    consFile = fopen("consistance.txt", "r");

    if(consFile == 0)
    {
        printf("Warning: Error reading consistance from the file!\n");
        pressEnterToContinue();
        return;
    }

    char part[100];
    while(true)
    {
        int resPart = fscanf(consFile, "%[^\n]%*c", part);
        if(resPart != 1) break;
        if(feof(consFile)) break;
        addPart(part);
        part[0] = '\0';
    }

    fclose(consFile);
}

void saveConsistance()
{
    FILE *consFile;
    consFile = fopen("consistance.txt", "w");

    if(consFile == 0)
    {
        printf("Error saving consistance to the file: can't open the file\n");
        pressEnterToContinue();
        return;
    }

    for(int i = 0; i < consistanceCount; i++)
    {
        if(fprintf(consFile, consistance[i]) < 0 ||
                fprintf(consFile, "\n") < 0)
        {
            printf("Error saving consistance to the file: "
                   "can't write some items\n");
            pressEnterToContinue();
            return;
        }
    }

    fclose(consFile);
}

void addPart(char *newPart)
{
    char *partToSave = (char *) calloc(strlen(newPart) + 1, sizeof(char));
    strcpy(partToSave, newPart);
    char **newArray = (char**) calloc(++consistanceCount, sizeof(char*));
    for(int i = 0; i < consistanceCount - 1; i++)
    {
        newArray[i] = consistance[i];
    }
    newArray[consistanceCount - 1] = partToSave;
    free(consistance);
    consistance = newArray;
}
