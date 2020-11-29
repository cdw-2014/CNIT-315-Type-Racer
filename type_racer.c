#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <curl/easy.h>

#define TRUE 1
#define FALSE 0

typedef struct Score {
    int time;
    char datetime[20];
    struct Score* next;
} Score;

char quotes[4][200] = { "Live in the sunshine.",
                    "He that, is of the opinion, money",
                    "When people talk, listen completely.",
                    "The summit of happiness"};

/* Function Prototypes */
int getMenuInput();
int calculateDistance(char*, char*);
int min(int, int, int);
//void drawGame();
char** getPassage();
void gameLoop();
//void getScore();

int main() {
    char* passage = getPassage();
    printf("%s", passage);
    int menuSelection = getMenuInput();
    switch (menuSelection)
    {
    case 1:
        gameLoop();
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
int calculateDistance(char *expected, char *actual) {
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
    return(matrix[strlen(actual)][strlen(expected)]);
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
void drawGame() {

}

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
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
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    chunk.size = 0;    /* no data at this point */
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/random/2");
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

    return chunk.memory;
}

void gameLoop() {
    //Quote* passages = getPassage();
    int count = 0; // count of passages completed
    int totalTime = 0, mistakes = 0;
    int c;
    while((c = getchar()) != '\n' && c != EOF);
    while (count < 4) {
        char input[200];
        printf("%s\n\n", quotes[count]);
        time_t startTime = time(NULL);
        fgets(input, 200, stdin);
        time_t finishTime = time(NULL);
        printf("Time: %ld\n", finishTime-startTime);
        totalTime += finishTime-startTime;

        /* Error calculation */
        mistakes += calculateDistance(quotes[count], input);
        count++;
        /* Calculate score */
    }
    printf("Total Time: %d\n", totalTime);
    printf("Total Mistakes: %d\n", mistakes);
}

// Chapter 10 data files
// Save Scores to file
// Output list of all scores
// Possibly have aggregate data at bottom
void getScore() {

}



