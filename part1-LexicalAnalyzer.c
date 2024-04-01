/*
 * This lexical analyzer recognizes the following tokens:
 *
 * Identifiers:       Sequences of letters and digits, starting with a letter.
 * Integer literals:  Sequences of digits.
 * Additive operators: '+' and '-'.
 * Multiplicative operators: '*' and '/'.
 * Parentheses:       '(' and ')'.
 * Semicolon:         ';' (extra rule, not initially required).
 * Equal operator:    '=' (extra rule, not initially required).
 *
 * Other characters are considered errors.
*/

//the packages needed
#include <stdio.h>
#include <ctype.h>

//Variables
char lexeme [100] = " "; //an array of characters that indicate a basic unit in program
char nextChar = ' '; //a char that reads from the input
int charClass = -1; // Checks if it can classify it as letter, digit, or other.
                    //only refers to LETTER, DIGIT, or UNKNOWN for the tokens
int nextToken = 0; // the classification of the lexeme
int lexLen = -1; //tracks the length of the lexeme
FILE* input_file; //the input file

//Character Classes - constants
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Token codes as constants; the number coresponding with the variable
#define ID 10 //Sequences of letters and digits, starting with a letter (e.g., x, name123)
#define INT_LIT 11 // Sequences of digits (e.g., 10, 456)
#define LEFT_PAREN 20 //Left '(' parenthesis.
#define RIGHT_PAREN 21 // Right ')' parenthesis.
#define ADD_OP 30 //Plus (+) symbol.
#define SUB_OP 31 //Minus (-) symbol.
#define MULT_OP 40 //Multiplication (*) symbol
#define DIV_OP 41 //Division (/) symbol.
#define SEMI_COLON 50 // Semicolon (;) symbol
#define EQUAL_OP 51 // Equal (=) symbol

//declaration of user-defined functions, minimized parameter passing
void addChar();
void getChar();
void getNonBlank();
int lookup(char ch);
int lex();

// Adds the next character to the lexeme, handling potential overflow.
void addChar() {

    // Check if lexLen is within the bounds of the lexeme array
    if (lexLen < sizeof(lexeme) - 1) {
        //auto-incremented the length of the lexeme
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';  // indicates new lexeme

    } else {
        // Handle potential overflow (lexeme is full)
        printf("Error: Lexeme array overflow\n");
    }
}

// Reads the next character from the input file and determines its character class.
void getChar(){

    //gets the first char
    nextChar = getc(input_file);

    //checks if it's the end-of-file
    if (nextChar != EOF) {

        //checks if it's a letter, digit, or other
        if(isalpha(nextChar)){
            charClass = LETTER;
        }else if (isdigit(nextChar)){
            charClass = DIGIT;
        }else{
            charClass = UNKNOWN;
        }
    } else {
        //end of the file
        charClass = EOF;
    }
}

// Skips whitespace characters.
void getNonBlank() {
    while (nextChar != NULL && isspace(nextChar)) // if its null or space, skip it
        getChar();
}

// Identifies tokens that are single characters and updates nextToken.
int lookup(char ch){
    //add the char to the lexeme
    addChar();

    //find what token the char is
    switch(ch){
        case '(':
            nextToken = LEFT_PAREN;
            break;

        case ')':
            nextToken = RIGHT_PAREN;
            break;

        case '+':
            nextToken = ADD_OP;
            break;

        case '-':
            nextToken = SUB_OP;
            break;

        case '*':
            nextToken = MULT_OP;
            break;

        case '/':
            nextToken = DIV_OP;
            break;
        case '=':
            nextToken = EQUAL_OP;
            break;
        case ';':
            nextToken = SEMI_COLON;
            break;
        default:
            nextToken = UNKNOWN; // unrecognized character error - CHECK
            printf("Error: unrecognized character '%d'\n", ch);
            break;

    }

    return nextToken;
}

// Implements the lexical analyzer state machine, updating lexeme and nextToken based on the character class.
int lex(){

    //intializes new lexeme by setting it to 0
    lexLen = 0;

    //skip white spaces
    getNonBlank();

    //if the charClass has already identified the lexeme, then add it,
    //otherwise, use the lookup to identify it, or end the function
    switch (charClass){

        case LETTER:
            addChar();
            getChar();

            //continues to go while it's considered an ID
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }

            nextToken = ID;
            break;
        case DIGIT:
            addChar();
            getChar();

            //continues until it isn't a digit anymore
            while(charClass == DIGIT){
                addChar();
                getChar();
            }

            nextToken = INT_LIT;
            break;

        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = '\0';
            break;

        default:
            nextToken = -3; // invalid character error
            printf("Error: invalid character class '%d'\n", charClass);
            break;
    }

    return nextToken;
}


// Opens the input file, calls `lex()` repeatedly until EOF is reached, and prints each token and lexeme.
int main(int argc, char *argv[]){

    //gets the inputed file - might need to fix this to work on any
    input_file = fopen("input.txt", "r");

    //checks if the file given is valid/not null
    if (input_file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    //gets the first char
    getChar();

    //while the file hasn't ended, keep getting the avaliable tokens
    while (nextToken != EOF) {
        lex();
        if (nextToken != EOF) {
            printf("Next token is: %-2d, Next lexeme is '%s'\n", nextToken, lexeme);
        }
    }

    //close file
    fclose(input_file);
    return 0;

}
