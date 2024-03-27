/*
A simple lexcial analyzer that can recognize the following tokens:
identifier, integer literal, left parenthesis, right parenthsis, additive operators, and multiplicative operators.
*/
//the packages needed
#include <stdio.h>
#include <ctype.h>
#include <direct.h>

//Variables
char lexeme [100] = " "; //an array of characters that indicate a basic unit in program
char nextChar = ' ';
int charClass = -1;// only refers to LETTER, DIGIT, or UNKNOWN for the tokens
int nextToken = 0; // the type of token it is, need to change back to -1?
int lexLen = -1;
FILE* input_file;

//Defining constants - Character Classes
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Token codes - constants
#define ID 10
#define INT_LIT 11
#define LEFT_PAREN 20
#define RIGHT_PAREN 21
#define ADD_OP 30
#define SUB_OP 31
#define MULT_OP 40
#define DIV_OP 41

//declaration of user-defined functions, minimized parameter passing
void addChar();
void getChar();
void getNonBlank();
int lookup(char ch);
int lex();

//adds the char to end of lexeme
void addChar() {
    // Check if lexLen is within the bounds of the lexeme array
    if (lexLen < sizeof(lexeme) - 1) {
        printf("adding new Char");
        //auto-incremented the length of the lexeme
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';  // indicates new lexeme
    } else {
        // Handle potential overflow (lexeme is full)
        printf("Error: Lexeme array overflow\n");
    }
}

// gets a char from the input file, saves it to nextChar, and decides the charClass - use the isDigit, isAlpha?
void getChar(){
    //gets the first char
    nextChar = getc(input_file);
    printf("getChar char: %d \n",nextChar);
    if (nextChar != EOF) {
        if(isalpha(nextChar)){
            charClass = LETTER;
            printf("Letter char \n");
        }else if (isdigit(nextChar)){
            charClass = DIGIT;
            printf("Digit char");
        }else{
            charClass = UNKNOWN;
            printf("Unkown char");
        }

        printf("Charclass: %d \n", charClass);
    } else {

        //don't use nextChar?
        charClass = EOF;
    }
}

//A function used to skip blank space
void getNonBlank() {
    while (nextChar != ' ' && isspace(nextChar))
        getChar();
}

//The function first calls addChar(), and then decide nextToken
//based on the parameter char using a switch statement.
int lookup(char ch){
    addChar();

    printf("lookup");

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
        default:
            nextToken = UNKNOWN; // unrecognized character error - CHECK
            printf("Error: unrecognized character '%d'\n", ch);
            break;

    }

    return nextToken;
}

// runs the state diagram to update the content of lexeme and nextToken
int lex(){

    //intializes new lexeme by setting it to 0
    lexLen = 0;

    //skip white spaces
    getNonBlank();

    printf("lex");

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


//executes the start of code logic
int main(int argc, char *argv[]){

    //in the wrong place
    printf("%s\n", getcwd(NULL, 0));

    //gets the inputed file - might need to fix this to work on any
    input_file = fopen("C:\\Users\\emmas\\Desktop\\Programming Languages Proj\\input.txt", "r");

    //checks if the file given is valid/not null
    if (input_file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    getChar();
    printf("Next token is: %-2d", nextToken);

    while (nextToken != EOF) {
        lex();
        if (nextToken != EOF) {
            printf("Next token is: %-2d, Next lexeme is '%s'\n", nextToken, lexeme);
        }
    }

    int size = sizeof(lexeme) / sizeof(lexeme[0]); // Get array size (excluding null terminator)

    //E O F is the output - is that supposed to be it?, should I put this somewhere else to debug?
    for (int i = 0; i < size - 1; i++) { // Loop through each character (excluding null terminator)
        printf("%c ", lexeme[i]);
    }

    fclose(input_file);
    return 0;

}
