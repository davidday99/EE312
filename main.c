#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "crosswordfuncs.h"

// find file
FILE* ChooseFile(char filename[]) {
    FILE *f;

    printf("Choose a crossword file (try \"crossword.txt\"!):\n");

    fgets(filename, 100, stdin);
    filename[strlen(filename) - 1] = NULL;  // insert sentinel in place of '\n'

    printf("\nYou chose file \"%s\"\n", filename);
    f = fopen(filename, "r");
    return f;
}

char** StoreMatrix(FILE* f, uint32_t row, uint32_t col) {
    char **p = (char**) malloc(row * sizeof(char*));
    for (uint32_t i = 0; i < row; i++) {
        p[i] = (char*) malloc(col * sizeof(char));
    }
    //check that allocation was successful, if not return NULL
    if (p == NULL) {
        return NULL;
    }

    // transfer file to dynamic matrix
    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            fscanf(f, "%c", &p[i][j]);
            if (p[i][j] == '\n') {
                j--;
            }
        }
    }
    // convert matrix to all uppercase letters
    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            if ((p[i][j] >= 'a') && (p[i][j] <= 'z')) {
                p[i][j] -= 0x20;
            }
        }
    }
    fclose(f);  // file has been transferred to matrix and can now be closed
    return p;
}

// function 1 - find max vertical word length
void LongestVertWord(char** p, uint32_t row, uint32_t col) {
    uint32_t wordlen = 0, templen = 0;
    char word[100];

    // first, find length of longest vertical word, store max length in "wordlen"
    for (uint32_t i = 0; i < col; i++) {
        for (uint32_t j = 0; j < row; j++) {
            for (uint32_t k = 0; (j < row) && (p[j][i] != '*'); k++, j++) {
                templen++;
            }
            if (templen > wordlen) {
                wordlen = templen;
                templen = 0;
            } else {
                templen = 0;
            }
        }
    }
    // second, print all words of this length
    for (uint32_t i = 0; i < col; i++) {
        for (uint32_t j = 0; j < row; j++) {
            for (uint32_t k = 0; (j < row) && (p[j][i] != '*'); k++, j++) {
                word[k] = p[j][i];
                templen++;
            }
            word[templen] = NULL;  // insert sentinel
            if (templen == wordlen) {
                printf("%s\n", word);
                // clear word array and reset templen to 0
                for (uint32_t counter = 0; counter < templen; counter++) {
                    word[counter] = 0;
                }
                templen = 0;
            } else {
                for (uint32_t counter = 0; counter < templen; counter++) {
                    word[counter] = 0;
                }
                templen = 0;
            }
        }
    }
    printf("\n\nDone!\n");
}

uint8_t CompareWord(char a[], char b[]) {
    uint8_t result = 1;
    for (int32_t index = 0; (a[index] != 0) || (b[index] != 0); index++) {
        if (a[index] != b[index]) {
            result = 0;
            break;
        }
    }
    return result;
}

// function 2
void FindWord(char** p, uint32_t row, uint32_t col) {
    char userword[100] = {0};  // will contain user inputted word
    char matword[100] = {0};  // will contain word from matrix to compare to user inputted word
    int32_t rownum[100] = {0};  // will contain row numbers in which a matched word is located
    int32_t colnum[100] = {0};  // will contain column numbers in which a matched word is located
    int8_t matchfound = 0;

    printf("Enter word to find:\n");

    // take user inputted word
    fgets(userword, 100, stdin);
    userword[strlen(userword) - 1] = NULL;  // insert sentinel in place of '\n'

    // convert array to all uppercase
    for (uint32_t arraylen = 0; arraylen < strlen(userword); arraylen++) {
        if ((userword[arraylen] >= 'a') && (userword[arraylen] <= 'z')) {
            userword[arraylen] -= 0x20;
        }
    }

    // look for vertical match in crossword puzzle
    printf("\nLooking for vertical matches...");
    for (uint32_t i = 0; i < col; i++) {
        for (uint32_t j = 0; j < row; j++) {
            for (uint32_t k = 0; (j < row) && (p[j][i] != '*'); k++, j++) {
                matword[k] = p[j][i];
                rownum[k] = j;
                colnum[k] = i;

                rownum[k + 1] = -1;  // insert sentinel
                colnum[k + 1] = -1;
            }
            // if match found, print columns/rows in which match is located
            if (CompareWord(userword, matword)) {
                matchfound = 1;

                printf("\n\"%s\" found on column %u, ", userword, colnum[0]);
                printf("rows ");
                for (uint32_t countrow = 0; rownum[countrow] != -1; countrow++) {
                    printf("%u ", rownum[countrow]);
                }

                // clear matword, rownum, and colnum arrays
                for (int32_t counter = 0, limit = (strlen(matword)); counter < limit; counter++) {
                    matword[counter] = 0;
                    rownum[counter] = 0;
                    colnum[counter] = 0;
                }
            } else {
                for (int32_t counter = 0, limit = (strlen(matword)); counter < limit; counter++) {
                    matword[counter] = 0;
                    rownum[counter] = 0;
                    colnum[counter] = 0;
                }
            }
        }
    }
    if (matchfound != 1) {  // if no matches found matchfound = 0, print statement
        printf("No vertical matches found for \"%s.\"\n", userword);
    }


    // look for horizontal matches
    matchfound = 0;
    printf("\n\nLooking for horizontal matches...");
    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            for (uint32_t k = 0; (j < col) && (p[i][j] != '*'); k++, j++) {
                matword[k] = p[i][j];
                rownum[k] = i;
                colnum[k] = j;

                rownum[k + 1] = -1;
                colnum[k + 1] = -1;
            }
            if (CompareWord(userword, matword)) {
                matchfound = 1;
                printf("\n\"%s\" found, contained on row %u, ", userword, rownum[0]);
                printf("columns ");
                for (uint32_t countcol = 0; colnum[countcol] != -1; countcol++) {
                    printf("%u ", colnum[countcol]);
                }

                // clear matword, rownum, and colnum arrays
                for (int32_t counter = 0, limit = (strlen(matword)); counter < limit; counter++) {
                    matword[counter] = 0;
                    rownum[counter] = 0;
                    colnum[counter] = 0;
                }
            } else {
                for (int32_t counter = 0, limit = (strlen(matword)); counter < limit; counter++) {
                    matword[counter] = 0;
                    rownum[counter] = 0;
                    colnum[counter] = 0;
                }
            }
        }
    }
    if (matchfound != 1) {
        printf("No horizontal matches found for \"%s.\"\n", userword);
    }
    printf("\n\nDone!\n");
}

// function 3
uint8_t CheckSymmetry(char** p, uint32_t rowloc, uint32_t totalcols, uint32_t asteriskcol) {
    uint8_t symmetric = 1;

    if (p[rowloc][totalcols - asteriskcol - 1] == '*') {  // if asterisk has symmetrical counterpart, return 0
        symmetric = 0;
    }
    return symmetric;
}

void VertSymmetry(char **p, uint32_t row, uint32_t col) {
    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            if (p[i][j] == '*') {
                if (CheckSymmetry(p, i, col, j)) {
                    printf("\nAsymmetrical \"*\" at row %u column %u.\n", i, j);
                }
            }
        }
    }
    printf("\n\nDone!\n");
}

// function 4 - on quit, free memory
void QuitProgram(char** p, uint32_t row) {
    for (uint32_t i = 0; i < row; i++) {
        free(p[i]);
    }
    free(p);
}
// load a sample test to mess around with
FILE* LoadTestProgram() {
    FILE *f;
    char testchoose;

    printf("Which test case would you like to run?\n");
    printf("1. Test 1: an all lower case crossword puzzle with vertically symmetrical black squares.\n");
    printf("2. Test 2: a crossword puzzle with only black squares. (???)\n");
    printf("3. Test 3: a completely blank crossword puzzle. :*(\n");

    scanf("%c", &testchoose);
    getchar();  // absorb '\n' character to keep input buffer clear

    if (testchoose == '1') {
        f = fopen("test1.txt", "r");
    } else if (testchoose == '2') {
        f = fopen("test2.txt", "r");
    } else if (testchoose == '3') {
        f = fopen("test3.txt", "r");
    } else {
        f = NULL;
    }
    return f;
}

int main() {
    char filename[100] = {0};
    FILE *f;
    uint32_t row, col;
    char loadprogram;
    char choosefunc;

    printf("Hello!\nPress 1 to load a sample test file. Press 2 to input a file name to load.\n");

    tryagain:
    scanf("%c", &loadprogram);
    getchar();  // absorb '\n' character

    if (loadprogram == '1') {
        do {
            f = LoadTestProgram();
            if (f == NULL) {
                printf("\nWhoops, invalid keypress! Please try again.\n");
            } else {
                printf("\nLoading test!\n");
            }
        } while (f == NULL);
    } else if (loadprogram == '2') {
        do {
            f = ChooseFile(filename);
            if (f == NULL) {
                printf("\nThat file could not be found :(\n\n");
            } else {
                printf("\nFile found!\n");
            }
        } while (f == NULL);
    } else {
        printf("\nUh-oh, invalid keypress! Please try again.\n");
        goto tryagain;
    }

    // get matrix dimensions
    fscanf(f, "%u, %u", &row, &col);
    printf("\n%u x %u Matrix\n\n", row, col);

    if ((row == 0) || (col == 0)) {
        printf("Error! No crossword puzzle to load! Quitting program...\n");
        goto endprogram;
    }

    // return pointer to dynamic matrix containing crossword puzzle, if allocation failed end program
    char **crossword = StoreMatrix(f, row, col);
    if (crossword == NULL) {
        printf("\n\nMemory allocation was unsuccessful. Quitting program...\n\n");
        goto endprogram;
    }

    // print crossword puzzle from matrix
    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            printf("%c ", crossword[i][j]);
        }
        printf("\n");
    }

    // ask user which function they would like to perform, run until user chooses to quit
    while (1) {
        printf("\nChoose which function you would like to perform.\n");
        printf("----------------------------------------------------------------------------------\n");
        printf("1. Find max vertical word length.\n");
        printf("2. Give a word and print all positions where it's found.\n");
        printf("3. Check if black squares (\"*\") are symmetric and print positions for those that aren't.\n");
        printf("4. Quit.\n");
        printf("\nNOTE: If you've pressed an invalid key too many times, press \"ENTER\" an equal\n"
                       "number of times to clear the input buffer.\n");
        printf("----------------------------------------------------------------------------------\n\n");

        scanf("%c", &choosefunc);
        getchar();  // absorb input buffer

        switch (choosefunc) {
            case '1':
                LongestVertWord(crossword, row, col);
                break;

            case '2':
                FindWord(crossword, row, col);
                break;

            case '3':
                VertSymmetry(crossword, row, col);
                break;

            case '4':
                QuitProgram(crossword, row);
                printf("\nGoodbye :)\n");
                goto endprogram;

            default:
                printf("Uh-oh, invalid keypress! Please try again.\n");
                break;
        }
    }
    endprogram:
    return 0;
}
