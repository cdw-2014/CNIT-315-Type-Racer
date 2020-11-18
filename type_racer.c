#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct Quote {
    char text[200];
} Quote;

typedef struct Score {
    int time;
    char datetime[20];
    struct Score* next;
} Score;


char quotes[4][200] = { "Live in the sunshine, swim the sea, drink the wild air.",
                    "He that is of the opinion money will do everything may well be suspected of doing everything for money.",
                    "When people talk, listen completely. Most people never listen.",
                    "The summit of happiness is reached when a person is ready to be what he is."};

/* Function Prototypes */
//float getInput();
int getMenuInput();
//void drawGame();
//Quote* getPassage();
//void gameLoop();
//void getScore();

int main() {
    printf("\n\ntest");
    getMenuInput();

    return EXIT_SUCCESS;
}


// placeholder. need to change.
// Validating that the text entered matches the passage.
// Too bad the only people who know how to run the country are busy driving cabs and cutting hair.
// Too bad the onby people who know how to run the country are busydriving cabs and cutting hair.
//               ^ mistake here                                    ^ and here
// We can use one of these:
// https://itnext.io/string-similarity-the-basic-know-your-algorithms-guide-3de3d7346227
float getInput() {
	do {
		if (1) {
		} else {
			printf("Invalid input. Please try again."); /* Else, print try again prompt */
		}
	} while (0); /* Loop when bound conditions are not met */
}

// detect input from user for menu options suchas start game, view scoreboard, and quit.
int getMenuInput() {
    int selection;
    printf("\n\nSelect an option:\n");
    printf("1 - Start New Game\n");
    printf("2 - Quit\n");
    scanf("%d", &selection);
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

//REST API call or pull from static list
Quote* getPassage() {

}

void gameLoop() {
    int quit = 0;
    do {
        //Quote* passages = getPassage();
        int count = 0; // count of passages completed
        while (count < 4) {
            char input[200] = "";
            printf("%s", quotes[count]);
            time_t startTime = time(NULL);
            scanf("%s", input);
            time_t finishTime = time(NULL);
            printf("Time:%ld\n", finishTime-startTime); 
            /* Calculate score */

        }
    } while(!quit);
}

// Chapter 10 data files
// Save Scores to file
// Output list of all scores
// Possibly have aggregate data at bottom
void getScore() {

}



