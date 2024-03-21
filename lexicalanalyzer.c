#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEXEME_LENGTH 100

//enumerator of types of tokens
enum Token {
  IDENTIFIER,
  INTEGER_LITERAL,
  LEFT_PAREN,
  RIGHT_PAREN,
  ADD_OP,
  SUB_OP,
  MUL_OP,
  DIV_OP,
  UNKNOWN
};


//gets the
void getChar(FILE* input_file, char* next_char) {
  *next_char = fgetc(input_file);
}

void addChar(char* lexeme, char next_char) {
  if (strlen(lexeme) < MAX_LEXEME_LENGTH - 1) {
    lexeme[strlen(lexeme)] = next_char;
    lexeme[strlen(lexeme)] = '\0';
  } else {
    printf("Error: Lexeme too long\n");
  }
}

enum Token lookup(char next_char, char* lexeme) {
  addChar(lexeme, next_char);
  switch (next_char) {
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
    case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case '_':
      return IDENTIFIER;
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
      return INTEGER_LITERAL;
    case '(':
      return LEFT_PAREN;
    case ')':
      return RIGHT_PAREN;
    case '+':
      return ADD_OP;
    case '-':
      return SUB_OP;
    case '*':
      return MUL_OP;
    case '/':
      return DIV_OP;
    default:
      return UNKNOWN;
  }
}

//finds if the next character is a blank
void getNonBlank(FILE* input_file, char* next_char) {
  while (next_char != NULL && isspace(*next_char)) {
    getChar(input_file, next_char);
  }
}


// The function lex() runs the state diagram to update the content of lexeme and nextToken according to the char class.
enum Token lex(FILE* input_file, char* lexeme) {
  char next_char;
  getChar(input_file, &next_char);
  while (next_char != EOF) {
    getNonBlank(input_file, &next_char);
    if (next_char == EOF) {
      return UNKNOWN; // End of file
    }
    enum Token token = lookup(next_char, lexeme);
    getChar(input_file, &next_char);
    return token;
  }
  return UNKNOWN; // End of file
}

//runs the program file
int main() {
  //gets the inputed file
  FILE* input_file = fopen("input.txt", "r");

  if (input_file == NULL) {
    printf("Error opening file\n");
    return 1;
  }

  char lexeme[MAX_LEXEME_LENGTH] = "";
  enum Token token;

  while ((token = lex(input_file, lexeme)) != UNKNOWN) {
    switch (token) {
      case IDENTIFIER:
        printf("Identifier: %s\n", lexeme);
        break;
      case INTEGER_LITERAL:
        printf("Integer Literal: %s\n", lexeme);
        break;
      case LEFT_PAREN:
        printf("Left Paren\n");
        break;
      case RIGHT_PAREN:
        printf("Right Paren\n");
        break;
      case ADD_OP:
        printf("Add Operator\n");
        break;
      case SUB_OP:
        printf("Subtraction Operator\n");
        break;
      case MUL_OP:
        printf("Multiplication Operator\n");
        break;
      case DIV_OP:
        printf("Division Operator\n");
        break;
      default:
        printf("Unknown token\n");
    }
    lexeme[0] = '\0'; // Reset lexe
}
