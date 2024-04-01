#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *input;
char token;

void factor();
void term();
void expr();

//Variables
char lexeme [100] = " "; //an array of characters that indicate a basic unit in program
char nextChar = ' ';
int charClass = -1; // Checks if it can classify it as letter, digit, or other.
                    //only refers to LETTER, DIGIT, or UNKNOWN for the tokens
int nextToken = 0; // the classification of the lexeme
int lexLen = -1; //tracks the length of the lexeme
FILE* input_file; //the input file

//Character Classes - constants
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

//extra - just in case
#define SEMI_COLON 50
#define EQUAL_OP 51

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
        //auto-incremented the length of the lexeme
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';  // indicates new lexeme

    } else {
        // Handle potential overflow (lexeme is full)
        printf("Error: Lexeme array overflow\n");
    }
}

// gets a char from the input file, saves it to nextChar, and decides the charClass
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

//A function used to skip blank space
void getNonBlank() {
    while (nextChar != NULL && isspace(nextChar)) // if its null or space, skip it
        getChar();
}

//The function first calls addChar(), and then decide nextToken
//based on the parameter char using a switch statement.
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

// runs the state diagram to update the content of lexeme and nextToken
int lex(){

    //intializes new lexeme by setting it to 0
    lexLen = 0;

    //skip white spaces
    getNonBlank();

    //if the charClass has already identified the lexeme, then add it,
    //otherwise, use the lookup to identify it
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


void error() {
    fprintf(stderr, "Error in parsing.\n");
    exit(1);
}

void match(char expectedToken) {
    if (token == expectedToken)
        token = *input;
    else
        error();
}

void id() {
    printf("[%c]", token);
    match(token);
}

void factor() {
    if (isalpha(token))
        id();
    else if (token == '(') {
        match('(');
        expr();
        match(')');
    } else
        error();
}

void term() {
    factor();
    while (token == '*' || token == '/') {
        printf("[%c] ", token);
        match(token);
        factor();
    }
}

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

int main() {
    input = malloc(100 * sizeof(char));
    printf("statement: ");

    scanf("%s", input);

    lex();
    expr();
    if (nextToken != EOF) {
        printf("Error: unexpected token '%s'\n", lexeme);
    }

    fclose(input);
    return 0;
}
