#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct terminal {
    int childCount;
    char* terminal;
} TERMINAL;

typedef struct threeChildren {
    int childCount;
    int nonTerminal;
    void *left, *middle, *right;
} THREE_CHILDREN;

typedef struct twoChildren {
    int childCount;
    int nonTerminal;
    void *left, *right;
} TWO_CHILDREN;

typedef struct oneChild {
    int childCount;
    int nonTerminal;
    void *child;
} ONE_CHILD;

typedef struct grammar {
    char** nonTerminals;
    int* sizes;
    int** childrenSizes;
    int*** nonterminalAncestors;
    char*** singleChild;
    int* terminalSizes;
} GRAMMAR;

THREE_CHILDREN* createThreeChildren(int nonTerminal) {
    THREE_CHILDREN *newNode = malloc(sizeof(THREE_CHILDREN));
    newNode->childCount = 3;
    newNode->nonTerminal = nonTerminal;
    newNode->left = NULL;
    newNode->middle = NULL;
    newNode->right = NULL;
    return newNode;
}

TWO_CHILDREN* createTwoChildren(int nonTerminal) {
    TWO_CHILDREN *newNode = malloc(sizeof(TWO_CHILDREN));
    newNode->childCount = 2;
    newNode->nonTerminal = nonTerminal;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

ONE_CHILD* createOneChild(int nonTerminal) {
    ONE_CHILD* newNode = malloc(sizeof(ONE_CHILD));
    newNode->nonTerminal = nonTerminal;
    newNode->childCount = 1;
    newNode->child = NULL;
    return newNode;
}

TERMINAL* createTerminal(char* value) {
    TERMINAL* newTerminal = malloc(sizeof(TERMINAL));
    newTerminal->terminal = value;
    newTerminal->childCount = 0;
    return newTerminal;
}

int getRandom(GRAMMAR* grammar, int nonterminal) {
    int limit = grammar->sizes[nonterminal];
    return rand() % limit;
}

void* generateTree(int nonterminal, GRAMMAR* grammar) {
    void* created;
    if (nonterminal == 0) {
        // Creating condition
        int constituentIndex = getRandom(grammar, nonterminal);
        THREE_CHILDREN* node = createThreeChildren(nonterminal);
        node->left = generateTree(grammar->nonterminalAncestors[0][constituentIndex][0], grammar);
        node->middle = generateTree(grammar->nonterminalAncestors[0][constituentIndex][1], grammar);
        node->right = generateTree(grammar->nonterminalAncestors[0][constituentIndex][2], grammar);
        created = node;
    } else if (nonterminal == 1) {
        int constituentIndex = getRandom(grammar, nonterminal);
        int childrenCount = grammar->childrenSizes[1][constituentIndex];

        if (childrenCount == 1) {
            ONE_CHILD* node = createOneChild(nonterminal);
            node->child = generateTree(grammar->nonterminalAncestors[1][2][0], grammar);
            created = node;
        } else if (childrenCount == 2) {
            TWO_CHILDREN* node = createTwoChildren(nonterminal);
            node->left = generateTree(grammar->nonterminalAncestors[1][1][0], grammar);
            node->right = generateTree(grammar->nonterminalAncestors[1][1][1], grammar);
            created = node;
        } else if (childrenCount == 3) {
            THREE_CHILDREN* node = createThreeChildren(nonterminal);
            node->left = generateTree(grammar->nonterminalAncestors[1][0][0], grammar);
            node->middle = generateTree(grammar->nonterminalAncestors[1][0][1], grammar);
            node->right = generateTree(grammar->nonterminalAncestors[1][0][2], grammar);
            created = node;
        }
    } else {
        int index = rand() % grammar->terminalSizes[nonterminal - 2];
        TERMINAL* node = createTerminal(grammar->singleChild[nonterminal - 2][index]);
        created = node;
    }
    return created;
}

void displayTree(void* node) {
   int childCount = ((ONE_CHILD*) node)->childCount;

   if (!childCount) {
       printf("%s", ((TERMINAL*) node)->terminal);
   } else if (childCount == 1) {
       displayTree(((ONE_CHILD*) node)->child);
   } else if (childCount == 2) {
       displayTree(((TWO_CHILDREN*) node)->left);
       printf("(");
       displayTree(((TWO_CHILDREN*) node)->right);
       printf(")");
   } else if (childCount == 3) {
       printf("(");
       displayTree(((THREE_CHILDREN*) node)->left);
       displayTree(((THREE_CHILDREN*) node)->middle);
       displayTree(((THREE_CHILDREN*) node)->right);
       printf(")");
   }
}

// Read the file character by character
char* readAllCharacters(char* filePath) {
    FILE *file;
    file = fopen(filePath, "r"); // Open file
    char* characters = malloc(sizeof(char)); // Dynamically allocate space
    int charCount = 0;
    int c = fgetc(file); // Get the char

    // Iterate
    while (c != EOF) {
        characters[charCount] = c;
        charCount++;
        characters = realloc(characters, sizeof(char) * (charCount + 1));
        c = fgetc(file);
    }

    characters[charCount] = '\0'; // Add end of file character
    fclose(file); // Close the file
    return characters;
}

// Parse the characters line by line
char** charsToLines(char* characters, int* size) {
    char** commandLines = malloc(sizeof(char*) * 2); // Allocate space
    char* line = strtok(characters, "\r\n"); // Get the part before the delimeter
    int i = 1;
    commandLines[0] = line;

    // Iterate
    while (line) {
        line = strtok(NULL, "\r\n"); // Get the next part
        commandLines[i] = line;
        i++;
        commandLines = realloc(commandLines, sizeof(char*) * (i + 1)); // Dynamically allocate the space
    }
    *size = i - 1;
    return commandLines;
}

int main() {
    char* PATH_OP = "./op";
    char* PATH_PRE_OP = "./pre_op";
    char* PATH_REL_OP = "./rel_op";
    char* PATH_SET_OP = "./set_op";
    char* PATH_VAR = "./var";

    char* COND = "<cond>";
    char* EXPR = "<expr>";
    char* OP = "<op>";
    char* PRE_OP = "<pre-op>";
    char* REL_OP = "<rel-op>";
    char* SET_OP = "<set-op>";
    char* VAR = "<var>";

    int cond1[] = {0, 5, 0};
    int cond2[] = {1, 4, 1};
    int expr1[] = {1, 2, 1};
    int exp2[] = {3, 1};
    int exp3[] = {6};

    int contChildren[] = {3, 3};
    int exprChildren[] = {3, 2, 1};
    int* childrenSizes[] = {contChildren, exprChildren};

    int* COND_ARR[] = {cond1, cond2};
    int* EXPR_ARR[] = {expr1, exp2, exp3};
    int** ANCESTORS[] = {COND_ARR, EXPR_ARR};

    int* terminalSizes = malloc(sizeof(int) * 5);

    int SIZE_OP = -1;
    int SIZE_PRE_OP = -1;
    int SIZE_REL_OP = -1;
    int SIZE_SET_OP = -1;
    int SIZE_VAR = -1;

    char** OP_ARR = charsToLines(readAllCharacters(PATH_OP), &SIZE_OP);
    terminalSizes[0] = SIZE_OP;
    char** PRE_OP_ARR = charsToLines(readAllCharacters(PATH_PRE_OP), &SIZE_PRE_OP);
    terminalSizes[1] = SIZE_PRE_OP;
    char** REL_OP_ARR = charsToLines(readAllCharacters(PATH_REL_OP), &SIZE_REL_OP);
    terminalSizes[2] = SIZE_REL_OP;
    char** SET_OP_ARR = charsToLines(readAllCharacters(PATH_SET_OP), &SIZE_SET_OP);
    terminalSizes[3] = SIZE_SET_OP;
    char** VAR_ARR = charsToLines(readAllCharacters(PATH_VAR), &SIZE_VAR);
    terminalSizes[4] = SIZE_VAR;

    int sizes[] = {2, 3, 1, 1, 1, 1, 1};
    char* names[7] = {COND, EXPR, OP, PRE_OP, REL_OP, SET_OP, VAR};
    char** single[] = {OP_ARR, PRE_OP_ARR, REL_OP_ARR, SET_OP_ARR, VAR_ARR};

    GRAMMAR grammar;
    grammar.nonTerminals = names;
    grammar.sizes = sizes;
    grammar.childrenSizes = childrenSizes;
    grammar.nonterminalAncestors = ANCESTORS;
    grammar.singleChild = single;
    grammar.terminalSizes = terminalSizes;

    srand(time(0));

    ONE_CHILD root;
    root.nonTerminal = -1;
    root.childCount = 1;
    root.child = generateTree(0, &grammar);
    printf("if(");
    displayTree(root.child);
    printf("){ }\n");
    return 0;
}
