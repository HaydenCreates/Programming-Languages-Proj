#include <stdio.h>
#include <stdlib.h>

// Define enum for grammar symbols
enum Symbol {
  ID,
  INT_LIT,
  PLUS,
  MINUS,
  MUL,
  DIV,
  LPAREN,
  RPAREN,
  ASSIGN,
  // Add more symbols if needed
  NONE
};

// Structure to represent a parse tree node
struct Node {
  enum Symbol symbol;
  struct Node* left;
  struct Node* right;
};

// Function prototypes
enum Symbol getNextToken();
struct Node* expr();
struct Node* term();
struct Node* factor();
struct Node* createNode(enum Symbol symbol);

// Global variable for storing the next token
enum Symbol next_token;

int main() {
  // Open file for output (replace "output.txt" with your desired filename)
  FILE* output_file = fopen("output.txt", "w");

  if (output_file == NULL) {
    printf("Error opening output file\n");
    return 1;
  }

  // Get the first token
  next_token = getNextToken();

  // Parse the expression
  struct Node* root = expr();

  // Print the parse tree in syntree format
  fprintf(output_file, "syntree:\n");
  printTree(root, output_file); // Implement printTree function to traverse and print the tree

  // Free the parse tree memory (implement a function to free allocated nodes)
  freeTree(root); // Implement freeTree function to recursively free memory

  fclose(output_file);
  return 0;
}

// Function to get the next token from the input (replace with your lexical analyzer code)
enum Symbol getNextToken() {
  // Implement logic to read next token from input and return its symbol
  // (This code depends on your specific lexical analyzer implementation)
  return NONE;  // Replace with actual token retrieval
}

// Function to parse an expression (assignment statement)
struct Node* expr() {
  if (next_token == ASSIGN) {
    // Create an assignment node
    struct Node* node = createNode(ASSIGN);
    next_token = getNextToken(); // Move past assignment operator

    // Parse the left-hand side (variable)
    node->left = factor();

    // Parse the right-hand side (expression)
    node->right = expr();

    return node;
  } else {
    // Parse the expression (non-assignment case)
    return term();
  }
}

// Function to parse a term
struct Node* term() {
  struct Node* node = factor();
  while (next_token == PLUS || next_token == MINUS) {
    enum Symbol op = next_token;
    next_token = getNextToken(); // Move past operator

    // Create an operator node
    struct Node* opNode = createNode(op);
    opNode->left = node;
    opNode->right = term();

    node = opNode;
  }
  return node;
}

// Function to parse a factor
struct Node* factor() {
  if (next_token == ID) {
    // Create an identifier node
    struct Node* node = createNode(ID);
    next_token = getNextToken(); // Move past identifier
    return node;
  } else if (next_token == INT_LIT) {
    // Create an integer literal node
    struct Node* node = createNode(INT_LIT);
    next_token = getNextToken(); // Move past literal
    return node;
  } else if (next_token == LPAREN) {
    next_token = getNextToken(); // Move past left parenthesis

    // Parse the expression within parentheses
    struct Node* node = expr();

    if (next_token != RPAREN) {
      printf("Error: Missing closing parenthesis\n");
      exit(1);
    }

    next_token = getNextToken(); // Move past right parenthesis
    return node;
  } else {
    printf("Error: Unexpected token in factor\n");
    exit(1);
  }
}

// Function to create a new parse tree node
struct Node* createNode(enum Symbol symbol) {
  struct Node* node = malloc(sizeof(struct Node));
  node->symbol = symbol;
  node->left = NULL;
  node->right = NULL;
  return node;
}

//
