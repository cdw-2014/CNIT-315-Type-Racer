/*
Name: CJ Weaver
Email: weave114@purdue.edu
Lab Time: Friday 12:30-1:20

Lab 4


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The following flags control whether the traverse function outputs the results
in the terminal. If set to NO_PRINT, the traverse function will only set the
current pointer to the end of the list. If set to YES_PRINT, the Scores' data
will also be logged.*/
#define YES_PRINT 1 // Turns on the printing option in the traverse function
#define NO_PRINT 0 // Turns on the printing option in the traverse function

/* The Score is the datatype of the entities in the linked list.
The typedef keyword prevents the need for redundantly referring
to "Score" as "struct Score". */
typedef struct Score {
    int score;
    int mistakes;
    time_t time;
    int quoteLength;
    time_t timestamp;
    struct Score* ptrNext;
} Score;

/* Initial function prototypes */
Score* createListNoScores();
Score* createListScore(int, int, time_t, int);
Score* traverseSteps(Score*, int);
Score* traverse(Score*, int);
Score* traverseBeforeLast(Score*);
Score* insertFront(Score*, Score*);
Score* insertEnd(Score*, Score*);
Score* insertMiddle(Score*, Score*, int);
Score* deleteFront(Score*);
Score* deleteEnd(Score*);
Score* deleteMiddle(Score*, int);
int lookUpByIndex(Score*, int);


/*
    Creates a pointer to linked list with no Scores
    @return Score* head 
*/
Score* createListNoScores() {
    Score* head = NULL; // Pointer to nothing that can be set to point to the head Score
    return head;
}

/*
    Creates a linked list Score
    @params firstName, lastName, puid, age
    @return Pointer to Score
*/
Score* createListScore(int score, int totalMistakes, time_t totalTime, int quoteLength) {
    Score *newScore = (Score*) malloc(sizeof(Score));
    newScore->score = score;
    newScore->mistakes = totalMistakes;
    newScore->time = totalTime;
    newScore->quoteLength = quoteLength;
    newScore->timestamp = time(0);
    newScore->ptrNext = NULL;
    return newScore;
}

/*
    Navigates through each Score in the linked list from the head to the last
    Score. Optionally prints the Score data to console.
    Used to insert to end and print.
    @params head Score, print option flag
    @return Pointer to last Score in linked list
*/
Score* traverse(Score* head, int isPrint) {
    Score* current = head; // points the current Score to the head Score
    Score* i = head; // points index Score to head Score
    while (i != NULL) { // loops through list starting with i until the end is reached
        /* If printing is enabled, print formatted data */
        if (isPrint) {
            printf("Score Address: %p\n", i); // Score's address
            printf("Timestamp: %ld\n", i->timestamp);
            printf("Score: %d\n", i->score); 
            printf("Total Mistakes: %d\n", i->mistakes); 
            printf("Total Time: %ld\n", i->time); 
            printf("Length of Quotes: %d\n", i->quoteLength); 
            printf("Next Address: %p\n", (i->ptrNext)); // Next Score's address. The address of the Score in the next loop will match this value.
            printf("-----------\n");
        }
        /* If the last Score is reached, point current to the Score */
        if (i->ptrNext == NULL) {
            current = i; // This is the pointer to the last Score
        }
        i = i->ptrNext; // the index Score pointer is still advanced so that the the last Score can be printed and the loop can be terminated.
    }
    return current;
}

/*
    Steps through the linked list from the head to the Score with index = steps.
    If the value passed in for steps exceeds the length of the linked list,
    the last Score will be returned instead.
    Used to insert/delete from middle.
    @params head Score, index
    @return Pointer to the Score at index = steps
*/
Score* traverseSteps(Score* head, int steps) {
    Score* i = head; // points index Score to head Score
    /* Loops from the head Score to Score before index or the last Score */
    while (steps-1 > 0 && i->ptrNext != NULL) {
        i = i->ptrNext; // advances the index pointer to next Score
        steps--; // decrements the amount of steps remaining
    }
    return i;
}

/*
    Navigates through each Score in the linked list from the head to the penultimate
    Score.
    Used to delete from end.
    @params head Score
    @return Pointer to penultimate Score in likned list
*/
Score* traverseBeforeLast(Score* head) {
    Score* current = head; // Points current Score to head
    /* Loops through until the next pointer of the next pointer is null */
    while (current->ptrNext->ptrNext != NULL) { // indicates that the penultimate Score has been reached.
        current = current->ptrNext; // advances that current Score in list
    }
    return current;
}

/*
    Adds a new Score to the front of the linked list
    @params head Score, new Score
    @return new head Score
*/
Score* insertFront(Score* head, Score* newScore) {
    newScore->ptrNext = head; // Points from the new Score to the old head Score
    head = newScore; // Reassigns the head Score to the new Score added before the old head
    return head;
}

/*
    Adds a new Score to the end of the linked list
    @params head Score, new Score
    @return new head Score
*/
Score* insertEnd(Score* head, Score* newScore) {
    newScore->ptrNext = NULL; // Points from the new Score to null because it is at the end
    Score* current = traverse(head, NO_PRINT); // navigates through the list and returns the pointer to the current last Score
    current->ptrNext = newScore; // Points from the current last Score to the new Score, adding it to the end of the list
    return head;
}

/*
    Adds a new Score to the middle of the linked list.
    @params head Score, new Score, index to insert new Score
    @return head Score
*/
Score* insertMiddle(Score* head, Score* newScore, int steps) {
    Score* current = traverseSteps(head, steps); // Steps through the list the amount of indices specified by steps param. Returns pointer to Score before that index.
    /* Checks if the returned pointer poitns to the last Score of the list */
    if (current->ptrNext == NULL) {
        insertEnd(head, newScore); // If it is the last Score, instead calls insertEnd()
    }
    else {
        newScore->ptrNext = current->ptrNext; // Points from the new Score to the current Scores next Score, adding it to the beginning of the last section of the list.
        current->ptrNext = newScore; // Points from the current list to the new Score, connecting the last section of the list to the first section.
    }
    return head;
}

/*
    Deletes a Score from the front of the linked list.
    @params head Score
    @return new head Score
*/
Score* deleteFront(Score* head) {
    Score* current = head; // Points the current Score to the head
    head = head->ptrNext; // replaces the head pointer to the second Score in the list
    current->ptrNext = NULL; // Reassigns the next pointer from the old head to null
    free(current); // deallocates the space used by the old head Score
    return head;
}

/*
    Deletes a Score from the end of the linked list.
    @params head Score
    @return head Score
*/
Score* deleteEnd(Score* head) {
    Score* last; // Declares the pointer to the new last Score
    Score* current = traverseBeforeLast(head); // navigates through the list and returns a pointer to the penultimate Score
    last = current->ptrNext; // assigns the last pointer to the penultimate Score
    current->ptrNext = NULL; // Removes link to old last Score
    free(current->ptrNext); // Deallocates space used by the old last Score
    return head;
}

/*
    Deletes a Score from the middle of the linked list at index = steps.
    @params head Score, index
    @return head Score
*/
Score* deleteMiddle(Score* head, int steps) {
    Score* current = traverseSteps(head, steps-1); // steps through the Scores and returns the Score at index = one before steps
    Score* temp; // declares a temp pointer

    /* Checks if Score at index is the last Score in list*/
    if (current->ptrNext == NULL) {
        deleteEnd(head); // If so, calls deleteEnd() instead
    }
    else { // Otherwise
        current->ptrNext = current->ptrNext->ptrNext; // replaces the pointer to the next Score with the Score 2 after the Score at index = steps
    }
    return head;
}

/*
    Searches the linked list for a Score by its PUID
    @params head Score, search param = puid
    @return index of resulting Score, or -1 if not found
*/
int lookUpByIndex(Score* head, int searchedScore) {
    Score* current = head; // creates a new pointer at the head of the list
    int index = 0; // starts an index counter at 0
    /* Loops through Scores until it reaches the lsat Score of the list */
    while (current != NULL) {
        if (current->score == searchedScore) { // if the Score's PUID = the search PUID
            return index; // return the index
        }
        index++; // Otherwise increment the counter
        current = current->ptrNext; // and advance to the next Score
    }
    return -1; // If not found after reaching the end, return -1.
} 