
/*
 * takes user inputted file name
 * returns pointer to file if found
 * returns null pointer if not found
 */
FILE* ChooseFile(char filename[]);

/*
 * dynamically allocates space on heap for matrix
 * stores crossword puzzle to matrix
 * closes file opened by ChooseFile as it is no longer needed
 */
char** StoreMatrix(FILE* f, uint32_t row, uint32_t col);

/*
 * reads matrix and determines length of longest word
 * once length is determined, all words of this length are printed, one per line
 */
void LongestVertWord(char** p, uint32_t row, uint32_t col);

/*
 * compares two strings
 * returns 1 if strings are equal
 * returns 0 if strings are not equal
 */
uint8_t CompareWord(char a[], char b[]);

/*
 * reads matrix vertically (column-major) and horizontally (row-major) for a word inputted by user
 * if match is found, all rows and columns in which word is located are printed
 * if match is not found, user is notified that no matches were found
 */
void FindWord(char** p, uint32_t row, uint32_t col);

/*
 * reads matrix and determines if asterisk placement is vertically symmetrical
 * calls CheckSymmetry
 */
void VertSymmetry(char **p, uint32_t row, uint32_t col);

/*
 * sub-function of VertSymmetry
 * checks if given asterisk has symmetrical counterpart
 * returns 1 if asymmetrical
 * returns 0 if symmetrical
 */
uint8_t CheckSymmetry(char** p, uint32_t rowloc, uint32_t totalcols, uint32_t asteriskcol);

/*
 * called when user chooses to end program
 * frees memory on heap
 */
void QuitProgram(char** p, uint32_t row);

/*
 * takes user inputted number between 1 and 3 and loads corresponding sample crossword puzzle
 */
FILE* LoadTestProgram();

