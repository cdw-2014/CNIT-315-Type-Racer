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

// Holds the quotes pulled from the quote sAPI and stores them in an array
typedef struct Passages {
    char arr[4][300];
} Passages;

/* Function Prototypes */
int getMenuInput();
int calculateDistance(char*, char*);
int min(int, int, int);
void drawGame(int*, int*);
Passages *getPassages();
Score* gameLoop(char[4][300]);
Score* getScores();
void saveScore(Score*);

// ASCII program title displayed when application is started
char title[7][121] = {
    " #######                        ######                                             #    #     #  #####  ###     #####  ",
    "    #    #   # #####  ######    #     #   ##    ####  ###### #####                # #   ##    # #     #  #     #     # ",
    "    #     # #  #    # #         #     #  #  #  #    # #      #    #              #   #  # #   # #        #     #       ",
    "    #      #   #    # #####     ######  #    # #      #####  #    #    #####    #     # #  #  #  #####   #     #       ",
    "    #      #   #####  #         #   #   ###### #      #      #####              ####### #   # #       #  #     #       ",
    "    #      #   #      #         #    #  #    # #    # #      #   #              #     # #    ## #     #  #     #     # ",
    "    #      #   #      ######    #     # #    #  ####  ###### #    #             #     # #     #  #####  ###     #####  "
};

int main() {
    // Prints ASCII title
    for (int i = 0; i < 7; i++) {
        printf("%s\n",title[i]);
    }
    // Past scores saved in linked list
    Score *head = (Score*) malloc(sizeof(Score)); // initializes linked list head
    head = getScores(); // laods the past scores from file
    do { // main loop
        int menuSelection = getMenuInput(); // prompts user for menu option
        switch (menuSelection)
        {
        case 1: ; // PLAY GAME
            Score *score = (Score*) malloc(sizeof(Score)); // initializes score for next game
            Passages *passages = getPassages(); // Fetches quotes from API
            score = gameLoop(passages->arr); // Calls main game logic loop and returns the score from game run
            saveScore(score); // saves the score returned from game loop
            break;
        case 2: ; // SHOW PAST SCORES
            traverse(head, TRUE); // traverses linked list and prints each scoreboard node
            break;
        case 3: // EXIT PROGRAM
            return EXIT_SUCCESS;
        default:
            return EXIT_SUCCESS;
        }
    } while(TRUE);
    return EXIT_SUCCESS;
}

// returns the minimum value from a, b, and c
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
// Validating that the text entered matches the passage.
// Too bad the only people who know how to run the country are busy driving cabs and cutting hair.
// Too bad the onby people who know how to run the country are busydriving cabs and cutting hair.
//               ^ mistake here                                    ^ and here
// Levenshtein distance algorithm
// @param actual string, expected string
int calculateDistance(char *s1, char *s2) {
    unsigned int x, y, s1len, s2len;
    s1len = strlen(s1); // length of actual string
    s2len = strlen(s2); // length of expected string
    unsigned int matrix[s2len+1][s1len+1]; // Levenshtein matrix
    matrix[0][0] = 0; // sets first distance cell to zero
    for (x = 1; x <= s2len; x++) // loops through x for each character of expected string
        matrix[x][0] = matrix[x-1][0] + 1; // sets distance equal to index of character
    for (y = 1; y <= s1len; y++) // loops through x for each character of actual string
        matrix[0][y] = matrix[0][y-1] + 1; // sets distance equal to index of character

    // loops through matrix
    for (x = 1; x <= s2len; x++) 
        for (y = 1; y <= s1len; y++) {
            // sets cell to the minimum edit distance between expected at index y and actual at index x
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));
        }
    //returns final distance
    return(matrix[s2len][s1len]);
}

// detect input from user for menu options such as start game, view scoreboard, and quit.
int getMenuInput() {
    int selection; // variable to store user input
    int isValid = FALSE;
    do {
        // displays options
        printf("\n\nSelect an option:\n");
        printf("1 - Start New Game\n");
        printf("2 - View Past Scores\n");
        printf("3 - Quit\n");
        if (scanf("%d", &selection) == 1) { // waits for user input and stores it in selection variable
            break; // breaks loop if input is value
        } else {
            printf("\nInvalid input. Please enter 1, 2, or 3.\n"); // error message
        }
    } while (!isValid); // continue loop
    return selection; // return selection
}


// Displaying the passage
// ASCII car or name
// Example:
// The is a passage. This is what you have to type.
//
//         *You %Score:xxxx
// 0%--------------------------------------------100%
//
void drawGame(int *quarter, int *score) {
    char car[4][15] = { // ASCII art for car
        "    ____",
        " __/  |_\\_",
        "|  _     _``-.",
        "'-(_)---(_)--'"
    };

    // lofic to print car in game
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < (*quarter)*15; j++) {
            printf(" "); // prints spaces before car depending on progress in game
        }
        printf("%s\n", car[i]); // prints car
    }
    for (int j = 0; j < (*quarter)*15; j++) {
            printf(" ");
        }
    printf("%d%% - Score: %d\n",(*quarter)*25, (*score)); // puts score under ASCII car
}

// holds data from quotes API
struct Quote {
  char *memory;
  size_t size;
};

// callback function for curl request
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

// REST API call
Passages *getPassages() {
    CURL *curl;
    CURLcode res;
    struct Quote chunk;
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    chunk.size = 0;    /* no data at this point */
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://cnit-315-quotes-api.herokuapp.com/random/5"); // gets data from api
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk); // sets data to chunk variable
        res = curl_easy_perform(curl); // performs request
        /* Check for errors */
        if(res != CURLE_OK) { // if not successful
            fprintf(stderr, "curl_easy_perform() failed: %sn", // print error
                curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl); // cleanup
    }
    curl_global_cleanup(); // close

    // String formatted as: ["This is a quote","Second quote","Here is another","Final string"]
    // Need to split into 4 strings
    char string[strlen(chunk.memory)]; // variable for quote data
    char result[4][300]; // array of quotes
    strcpy(string, chunk.memory); // copies unparsed data from chunk to string variable
    int index = 0; // index counter
    memmove (string, string+2, strlen (string+1) + 1); // cuts off the first two characters of string: ["
    while (index < 4) { // loops 4 times
        char *separator = strstr(string, "\",\""); // sets pointer to character at first instance of delimiter: ","
        if (separator != NULL) { 
            separator[0] = '\0'; // sets delimitor to null char
            strcpy(result[index], string); // copies quote string into array index i
            strcpy(string, separator+3); // cuts current quote string off of string quote and loops, cutting top quote off each time and assigning it to array
        }
        index++; // increment index
    }
    Passages *passages = (Passages*) malloc(sizeof(Passages)); // allocates memory for passages
    // copies passages into passage struct
    strcpy(passages->arr[0], result[0]);
    strcpy(passages->arr[1], result[1]);
    strcpy(passages->arr[2], result[2]);
    strcpy(passages->arr[3], result[3]);
   return passages; // returns passages
}

// main game logic
Score* gameLoop(char quotes[4][300]) {
    int count = 0; // count of passages completed
    int totalMistakes = 0; // total mistakes across all quotes
    int score = 0; // total score over all quotes
    int quoteLength = 0; // total length of all quotes
    time_t totalTime = 0; // total seconds taken typing across all quotes
    int c;
    char* p;
    while((c = getchar()) != '\n' && c != EOF); // consumes new line chars before prompting for input
    while (count < 4) { // loops for each quote
        drawGame(&count, &score); // draws car
        quoteLength += strlen(quotes[count]); // adds length of quote
        char input[200] = ""; // initializes user input
        printf("%s\n\n", quotes[count]); // prints quote
        time_t startTime = time(0); // sets begin time
        fgets(input, 200, stdin); // gets user input
        if ((p=strchr(input, '\n')) != NULL) // Checks for new line character (enter key) and assigns pointer to it
            *p = '\0'; // Assigns the string termination character to the end of the input
        time_t finishTime = time(0); // sets end time
        time_t elapsedTime = finishTime-startTime+1; // calculates time taken to type
        printf("Time: %ld\n", elapsedTime); // prints elapsed time
        totalTime += elapsedTime; // adds time to total

        /* Error calculation */
        int runMistakes = calculateDistance(quotes[count], input); // calucates mistakes
        totalMistakes += runMistakes; // adds run mistakes to total mistakes
        printf("Mistakes: %d\n", runMistakes); // prints mistakes
        count++; // increments counter
        /* Calculate score */
        // base score 2000
        // adds or subtracts in increments of 500 depending on if typing speed is faster or lower than the average 200 chars per 60 seconds
        // multiples score by modifier for % of mistakes
        score += ceil((2000 + 500*((quoteLength/elapsedTime) - (200/60))) * (    quoteLength - (2 * runMistakes)    )/quoteLength); //adds run score to total
    }
    drawGame(&count, &score); // after game, draw car once more to show at 100% progress and final score
    printf("Total Time: %ld\n", totalTime);
    printf("Total Mistakes: %d\n", totalMistakes);
    Score *newScore = (Score*) malloc(sizeof(Score)); // allocates space for new score
    // sets all values to score struct
    newScore->score = score;
    newScore->mistakes = totalMistakes;
    newScore->time = totalTime;
    newScore->quoteLength = quoteLength;
    newScore->timestamp = time(0);
    newScore->ptrNext = NULL;
    return newScore; // returns score
}

// Read scores from scoreboard file
Score* getScores() {
    FILE *scoreFile = fopen("./scoreboard.txt", "r"); // sets file to read
    // declares varaibles to pull from file
    time_t timestamp, time;
    int score, mistakes, quoteLength;
    Score *head = NULL; // linked list head
    while (fscanf(scoreFile, "%ld,%d,%d,%ld,%d", &timestamp, &score, &mistakes, &time, &quoteLength) != EOF) { // scans line of file (single score)
        if (head == NULL) { // if it is the first score
            head = createListScore(score, mistakes, time, quoteLength); // set head of list to first score
        } else {
            insertEnd(head, createListScore(score, mistakes, time, quoteLength)); // insert score to end of list
        }
    }
    fclose(scoreFile); // close file stream
    traverse(head, FALSE); // navigates through list and prints each node
    return head; // returns head of linked list
}

// Saves score to scoreboard file
void saveScore(Score* score) {
    FILE *scoreFile = fopen("./scoreboard.txt", "a"); // sets file to edit / create if it doesn't exist
    // prints values to file
    fprintf(scoreFile, "%ld,", score->timestamp);
    fprintf(scoreFile, "%d,", score->score);
    fprintf(scoreFile, "%d,", score->mistakes);
    fprintf(scoreFile, "%ld,", score->time);
    fprintf(scoreFile, "%d\n", score->quoteLength);
    fclose(scoreFile); // closes file
}




