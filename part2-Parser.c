/*
This parser implements the following grammar rules:
Expr → Term (+/- Term)
expr() function: calls term(), checks for ADD_OP or SUB_OP, calls term() again for each occurrence.

*Term → Factor (|/ Factor)
term() function: calls factor(), checks for MULT_OP or DIV_OP, calls factor() again for each occurrence.

Factor → id | intLit | ( Expr )
factor() function: checks for ID, INT_LIT, LEFT_PAREN, handles each case accordingly.
ID: calls lex() to get the variable name.
INT_LIT: calls lex() to get the integer value.
LEFT_PAREN: calls lex(), calls expr(), checks for RIGHT_PAREN, reports error if missing.

id → letter (letter | digit)
lex() function (part of factor handling): accumulates letters and digits into lexeme while charClass is LETTER or DIGIT.

intLit → digit (digit)
lex() function (part of factor handling): accumulates digits into lexeme while charClass is DIGIT.
*/

//packages
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Global variables
char lexeme[100] = "";//an array of characters that indicate a basic unit in program
char nextChar = ' '; //a char that reads from the input
int nextToken = -1;
int charClass = -1; // Checks if it can classify it as letter, digit, or other.
                    //only refers to LETTER, DIGIT, or UNKNOWN for the tokens
FILE *input; // File pointer for input
FILE *output; //the output file

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

//declaration of user-defined functions, minimized parameter passing
void expr();
void term();
void factor();
void addChar();
void getChar();
void getNonBlank();
int lookup(char ch);
int lex();

// Adds the next character to the lexeme, handling potential overflow.
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

//Skips whitespace characters.
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

// Identifies tokens that are single characters and updates nextToken.
int lookup(char ch) {

    //Implements grammar rule for other lexemes:
    // -finds the token if it's not considered an identifer
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
            fprintf(output, "Error: unrecognized character '%c'\n", ch);
            break;
    }
    return nextToken;
}

// Implements the lexical analyzer state machine, updating lexeme and nextToken based on the character class.
int lex() {
    memset(lexeme, 0, sizeof(lexeme));

    //skip white spaces
    getNonBlank();

    //if the charClass has already identified the lexeme, then add it,
    //otherwise, use the lookup to identify it, or end the function
    switch (charClass) {

        // Implements the grammar rule for identifiers:
        // - Must start with a letter
        // - Can be followed by letters or digits
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
            fprintf(output, "Error: invalid character class '%d'\n", charClass);
            break;
    }
    return nextToken;
}

// Handles the expression rule, calling term() for each operand and
//checking for addition or subtraction operators in between.
void expr() {
    fprintf(output, "[expr\n");
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        switch (nextToken) {
            case ADD_OP:
                fprintf(output, "[+]\n");
                break;
            case SUB_OP:
                fprintf(output, "[-]\n");
                break;
            default:
                fprintf(output, "[Error: unrecognized operator]\n");
                break;
        }
        lex();
        term();
    }
    fprintf(output, "]\n");
}

//Handles the term rule, calling factor() for each operand and
//checking for multiplication or division operators in between.
void term() {
    fprintf(output, "[term\n");
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        switch (nextToken) {
            case MULT_OP:
                fprintf(output, "[*]\n");
                break;
            case DIV_OP:
                fprintf(output, "[/]\n");
                break;
            default:
                fprintf(output, "[Error: unrecognized operator]\n");
                break;
        }
        lex();
        factor();
    }
    fprintf(output, "]\n");
}


//Handles the factor rule, checking for identifiers,
//integer literals, or parenthesized expressions. It calls lex() to
//get the lexeme (variable name or integer value) when needed.
void factor() {
    fprintf(output, "[factor\n");
    if (nextToken == ID) {
        fprintf(output, "[id [%s]]\n", lexeme);
        lex();
    } else if (nextToken == INT_LIT) {
        fprintf(output, "[intLit [%s]]\n", lexeme);
        lex();
    } else if (nextToken == LEFT_PAREN) {
        fprintf(output, "[(]\n");
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            fprintf(output, "[)]\n");
            lex();
        } else {
            fprintf(output, "Error: missing closing parenthesis\n");
        }
    } else {
        fprintf(output, "Error: invalid factor\n");
    }
    fprintf(output, "]\n");
}

// Executes the parser code
int main(int argc, char *argv[]) {
   //gets the inputed file
    input = fopen("input.txt", "r");

    output = fopen("output.txt", "w");

    if (output == NULL) {
        printf("Error opening file!\n");
        return 1; // Or exit the program in another way
    }

    //checks if the file given is valid/not null
    if (input == NULL) {
        fprintf(output, "Error opening file\n");
        return 1;
    }

    lex();
    expr();
    if (nextToken != EOF) {
        fprintf(output, "Error: unexpected token '%s'\n", lexeme);
    }

    fclose(output);
    fclose(input); // Close the input file
    return 0;
}
