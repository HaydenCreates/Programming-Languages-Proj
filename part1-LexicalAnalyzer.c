/*
A simple lexcial analyzer that can recognize the following tokens:
identifier, integer literal, left parenthesis, right parenthsis, additive operators, and multiplicative operators.
*/
//the packages needed
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//Global Variables - check
char lexeme [100] = " "; //an array of characters that indicate a basic unit in program
char nextChar = ' ';
int charClass = -1;// only refers to LETTER, DIGIT, or UNKNOWN for the tokens
int nextToken = -1; // the type of token it is
int lexLen = -1;

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

//adds the char to end of lexeme - Check
void addChar() {
    int len = strlen(lexeme);
    lexeme[len] = nextChar;
    lexeme[len+1] = '\0';
}

// gets a char from the input file, saves it to nextChar, and decides the charClass - use the isDigit, isAlpha?
void getChar(){

}

//A function used to skip blank space
void getNonBlank() {
    while (nextChar != NULL && isspace(nextChar))
        getChar();
}

//The function first calls addChar(), and then decide nextToken
//based on the parameter char using a switch statement.
int lookup(char ch){
    addChar();

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
            nextToken = LEFT_PAREN;
            break;
        default:
            nextToken = UNKNOWN; // unrecognized character error - CHECK
            printf("Error: unrecognized character '%c'\n", ch);
            break;
    }

    return nextToken;
}

int lex(){

    //what do I do with this?
    lexLen = 0;

    //skip white spaces
    getNonBlank();

    switch (charClass){

        case LETTER:
            addChar();
            getChar();

            //continues since it can be an identifier with letters and numbers
            while(charClass == LETTER || charClass == DIGIT){
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
