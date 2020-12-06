#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "linked_list.h"

#define TRUE 1
#define FALSE 0
/*
typedef struct Score {
    int score;
    int mistakes;
    time_t time;
    int quoteLength;
    time_t timestamp;
    struct Score* next;
} Score;
*/
char quotes[2][200] = { 
                    "Your time is limited, so don't waste it living someone else's life.",
                    "The way to get started"};

/* Function Prototypes */
int getMenuInput();
int calculateDistance(char*, char*);
int min(int, int, int);
void drawGame(int*, int*);
char** getPassage();
Score* gameLoop();
Score* getScores();
void saveScore(Score*);

int main() {
    char** passage = getPassage();
    Score *head = (Score*) malloc(sizeof(Score));
    head = getScores();
    int menuSelection = getMenuInput();
    switch (menuSelection)
    {
    case 1: ;
        Score *score = (Score*) malloc(sizeof(Score));
        score = gameLoop();
        saveScore(score);
        break;
    case 2:
        return EXIT_SUCCESS;
    default:
        break;
    }
    return EXIT_SUCCESS;
}

int min(int a, int b, int c) {	
	if(a <= b && a <= c)
	{
		return a;
	}
	else if(b <= a && b <= c)
	{
		return b;
	}
	else if(c <= a && c <= b)
	{
		return c;
	}
}
// placeholder. need to change.
// Validating that the text entered matches the passage.
// Too bad the only people who know how to run the country are busy driving cabs and cutting hair.
// Too bad the onby people who know how to run the country are busydriving cabs and cutting hair.
//               ^ mistake here                                    ^ and here
// We can use one of these:
// https://itnext.io/string-similarity-the-basic-know-your-algorithms-guide-3de3d7346227
int calculateDistance(char *s1, char *s2) {
    /*
    int x, y;
    int matrix[strlen(expected)+1][strlen(actual)+1];
    matrix[0][0] = 0;
    for (x = 1; x <= strlen(actual); x++) {
        matrix[x][0] = matrix[x-1][0] + 1;
    }
    for (y = 1; y <= strlen(expected); y++) {
        matrix[0][y] = matrix[0][y-1] + 1;
    }
    for (x = 1; x <= strlen(actual); x++)
        for (y = 1; y <= strlen(expected); y++)
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (expected[y-1] == actual[x-1] ? 0 : 1));
    for (int i = 0; i <= strlen(actual); i++) {
        printf("\n");
        for (int j = 0; j <= strlen(expected); j++ ) {
            printf("%2d ", matrix[i][j]);
        }
    }

    return(matrix[strlen(actual)][strlen(expected)]);
    */
   unsigned int x, y, s1len, s2len;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;
    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;
    for (x = 1; x <= s2len; x++)
        for (y = 1; y <= s1len; y++)
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));

    return(matrix[s2len][s1len]);
}

// detect input from user for menu options suchas start game, view scoreboard, and quit.
int getMenuInput() {
    int selection;
    int isValid = FALSE;
    do {
        printf("\n\nSelect an option:\n");
        printf("1 - Start New Game\n");
        printf("2 - Quit\n");
        scanf("%d", &selection);
        if (selection == 1 || selection == 2) {
            isValid = TRUE;
        } else {
            printf("Invalid input. Please Try again. Enter 1 or 2.\n\n");
        }
    } while (!isValid);
    return selection;
}


// Displaying the passage
// ASCII car or name
// Example:
// The is a passage. This is what you have to type.
//
//         *You %Correct/Time
//                        *PB
//               *Avg
// 0%--------------------------------------------100%
//
void drawGame(int *quarter, int *score) {
    char car[4][15] = {
        "    ____",
        " __/  |_\\_",
        "|  _     _``-.",
        "'-(_)---(_)--'"
    };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < (*quarter)*15; j++) {
            printf(" ");
        }
        printf("%s\n", car[i]);
    }
    for (int j = 0; j < (*quarter)*15; j++) {
            printf(" ");
        }
    printf("%d%% - Score: %d\n",(*quarter)*25, (*score));
}

struct Quote {
  char *memory;
  size_t size;
};

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct Quote *mem = (struct Quote *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

//REST API call or pull from static list
char** getPassage() {
    CURL *curl;
    CURLcode res;
    struct Quote chunk;
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    chunk.size = 0;    /* no data at this point */
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/random/4");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %sn", 
                curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    // ['rhryhgr e','erg','ergerg','asdasd arf ase r']

    printf("1");
    char results[4][300] = {"","","",""};
    printf("1");
    char unparsed[1200] = "";
    printf("1");
    strcpy(unparsed, *(chunk.memory));
    printf("1");
    char *token = strtok(unparsed, "\",\"");
    printf("1");
    int index = 0;
    while (token != NULL) {
        token = strtok(NULL, "\",\"");
        strcpy(results[index], *token);
        index++;
    }
    printf("\n\n\n");
    printf("%s", results[0]);
    printf("%s", results[1]);
    return &(results);
}

Score* gameLoop() {
    //char passages[4][300] = getPassage();
    int count = 0; // count of passages completed
    int totalMistakes = 0;
    int score = 0;
    int quoteLength = 0;
    time_t totalTime = 0;
    int c;
    char* p;
    while((c = getchar()) != '\n' && c != EOF);
    while (count < 2) {
        drawGame(&count, &score);
        quoteLength += strlen(quotes[count]);
        char input[200] = "";
        printf("%s\n\n", quotes[count]);
        time_t startTime = time(0);
        fgets(input, 200, stdin);
        if ((p=strchr(input, '\n')) != NULL) // Checks for new line character (enter key) and assigns pointer to it
            *p = '\0'; // Assigns the string termination character to the end of the input
        time_t finishTime = time(0);
        time_t elapsedTime = finishTime-startTime;
        printf("Time: %ld\n", elapsedTime);
        totalTime += elapsedTime;

        /* Error calculation */
        int runMistakes = calculateDistance(quotes[count], input);
        totalMistakes += runMistakes;
        printf("Mistakes: %d\n", runMistakes);
        count++;
        /* Calculate score */
        // (strlen / time)*100 - mistakes
        score += (1000 - 20 * (runMistakes * elapsedTime));
    }
    drawGame(&count, &score);
    printf("Total Time: %ld\n", totalTime);
    printf("Total Mistakes: %d\n", totalMistakes);
    Score *newScore = (Score*) malloc(sizeof(Score));
    newScore->score = score;
    newScore->mistakes = totalMistakes;
    newScore->time = totalTime;
    newScore->quoteLength = quoteLength;
    newScore->timestamp = time(0);
    newScore->ptrNext = NULL;
    return newScore;
}

// Chapter 10 data files
// Save Scores to file
// Output list of all scores
// Possibly have aggregate data at bottom
Score* getScores() {
    FILE *scoreFile = fopen("./scoreboard.txt", "r");
    time_t timestamp, time;
    int score, mistakes, quoteLength;
    Score *head = NULL;
    while (fscanf(scoreFile, "%ld,%d,%d,%ld,%d", &timestamp, &score, &mistakes, &time, &quoteLength) != EOF) {
        if (head == NULL) {
            head = createListScore(score, mistakes, time, quoteLength);
        } else {
            insertEnd(head, createListScore(score, mistakes, time, quoteLength));
        }
    }
    fclose(scoreFile);
    traverse(head, TRUE);
    return head;
}

void saveScore(Score* score) {
    FILE *scoreFile = fopen("./scoreboard.txt", "a");
    fprintf(scoreFile, "%ld,", score->timestamp);
    fprintf(scoreFile, "%d,", score->score);
    fprintf(scoreFile, "%d,", score->mistakes);
    fprintf(scoreFile, "%ld,", score->time);
    fprintf(scoreFile, "%d\n", score->quoteLength);
    fclose(scoreFile);
}




