#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Global variables
char lexeme[100] = "";
char nextChar = ' ';
int nextToken = -1;
int charClass = -1;
FILE *input; // File pointer for input

// Character classes
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Token codes
#define ID 10
#define INT_LIT 11
#define LEFT_PAREN 20
#define RIGHT_PAREN 21
#define ADD_OP 30
#define SUB_OP 31
#define MULT_OP 40
#define DIV_OP 41

// Function prototypes
void expr();
void term();
void factor();
void getChar(); // Added getChar() function prototype

// Utility functions

// Adds the next character to the lexeme, handling potential overflow.
void addChar() {
    int len = strlen(lexeme);
    lexeme[len] = nextChar;
    lexeme[len+1] = '\0';
}

// Reads the next character from the input file and determines its character class.
void getChar() {
    nextChar = getc(input);
    if (feof(input)) {
        charClass = EOF;
    } else if (isalpha(nextChar)) {
        charClass = LETTER;
    } else if (isdigit(nextChar)) {
        charClass = DIGIT;
    } else {
        charClass = UNKNOWN;
    }
}

// Skips whitespace characters.
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

// Identifies tokens that are single characters and updates nextToken.
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = -2; // unrecognized character error
            printf("Error: unrecognized character '%c'\n", ch);
            break;
    }
    return nextToken;
}

// Implements the lexical analyzer state machine, updating lexeme and nextToken based on the character class.
int lex() {
    memset(lexeme, 0, sizeof(lexeme));
    getNonBlank();
    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = ID;
            break;
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
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

// Recursive descent parser functions
void expr() {
    printf("[expr\n");
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        printf("[%c]\n", nextChar);
        lex();
        term();
    }
    printf("]\n");
}

void term() {
    printf("[term\n");
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        printf("[%c]\n", nextChar);
        lex();
        factor();
    }
    printf("]\n");
}

void factor() {
    printf("[factor\n");
    if (nextToken == ID) {
        printf("[id [%s]]\n", lexeme);
        lex();
    } else if (nextToken == INT_LIT) {
        printf("[intLit [%s]]\n", lexeme);
        lex();
    } else if (nextToken == LEFT_PAREN) {
        printf("[(]\n");
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            printf("[)]\n");
            lex();
        } else {
            printf("Error: missing closing parenthesis\n");
        }
    } else {
        printf("Error: invalid factor\n");
    }
    printf("]\n");
}

int main(int argc, char *argv[]) {
    printf("Enter the expression: ");
    input = stdin; // Initializing the input file pointer with stdin

    lex();
    expr();
    if (nextToken != EOF) {
        printf("Error: unexpected token '%s'\n", lexeme);
    }

    fclose(input); // Close the input file
    return 0;
}

