#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 1024

// -------------------- RULES --------------------
// Keywords
const char *KEYWORDS[] = {"if", "else", "while", "return", "func"};
int KEYWORDS_COUNT = 5;

// Special Identifiers (Three Students)
const char *SPECIAL_NAMES[] = {
    "Fojael",
    "Kawser",
    "Foyjur"
};

const char *SPECIAL_IDS[] = {
    "222", // Fojael → 22203222
    "221", // Kawser → 22203221
    "167"  // Foyjur → 22203167
};
int SPECIAL_COUNT = 3;

// Operators
const char *OPERATORS[] = {"+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">="};
int OPERATORS_COUNT = 10;

// -------------------- HELPERS --------------------
int isKeyword(const char *lex) {
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (strcmp(lex, KEYWORDS[i]) == 0) return 1;
    }
    return 0;
}

int isOperator(const char *lex) {
    for (int i = 0; i < OPERATORS_COUNT; i++) {
        if (strcmp(lex, OPERATORS[i]) == 0) return 1;
    }
    return 0;
}

int isSpecialName(const char *lex) {
    for (int i = 0; i < SPECIAL_COUNT; i++) {
        if (strcmp(lex, SPECIAL_NAMES[i]) == 0) return 1;
    }
    return 0;
}

int isSpecialID(const char *lex) {
    for (int i = 0; i < SPECIAL_COUNT; i++) {
        if (strcmp(lex, SPECIAL_IDS[i]) == 0) return 1;
    }
    return 0;
}

// -------------------- TOKEN CLASSIFIER --------------------
void classifyToken(const char *lexeme) {
    if (strlen(lexeme) == 0) return;

    // --- Special Names ---
    if (isSpecialName(lexeme)) {
        printf("<IDENTIFIER_NAME, %s>\n", lexeme);
        return;
    }

    // --- Special IDs ---
    if (isSpecialID(lexeme)) {
        printf("<ID_LAST3, %s>\n", lexeme);
        return;
    }

    // --- Keyword ---
    if (isKeyword(lexeme)) {
        printf("<KEYWORD, %s>\n", lexeme);
        return;
    }

    // --- Identifier ---
    if (isdigit(lexeme[0])) {
        // must start with one of the last-3 ID digits
        int valid = 0;
        for (int i = 0; i < SPECIAL_COUNT; i++) {
            if (strncmp(lexeme, SPECIAL_IDS[i], strlen(SPECIAL_IDS[i])) == 0) {
                valid = 1;
                break;
            }
        }
        if (valid) {
            printf("<IDENTIFIER, %s>\n", lexeme);
        } else {
            printf("<ERROR, Invalid identifier: %s>\n", lexeme);
        }
        return;
    }

    // --- Number ---
    if (isdigit(lexeme[0]) || lexeme[0] == '.') {
        int dotCount = 0, expCount = 0;
        for (int i = 0; i < strlen(lexeme); i++) {
            if (lexeme[i] == '.') dotCount++;
            if (lexeme[i] == 'e' || lexeme[i] == 'E') expCount++;
        }
        if (dotCount > 1 || expCount > 1) {
            printf("<ERROR, Malformed number: %s>\n", lexeme);
        } else {
            printf("<NUMBER, %s>\n", lexeme);
        }
        return;
    }

    // --- String Literal ---
    if (lexeme[0] == '$') {
        int len = strlen(lexeme);
        if (len >= 2 && lexeme[len - 1] == '$') {
            char strVal[MAX_LEN];
            strncpy(strVal, lexeme + 1, len - 2);
            strVal[len - 2] = '\0';
            printf("<STRING, %s>\n", strVal);
        } else {
            printf("<ERROR, Unterminated string: %s>\n", lexeme);
        }
        return;
    }

    // --- Operators ---
    if (isOperator(lexeme)) {
        printf("<OPERATOR, %s>\n", lexeme);
        return;
    }

    // --- Comments ---
    if (strncmp(lexeme, "//", 2) == 0) {
        printf("<COMMENT, %s>\n", lexeme);
        return;
    }
    if (strncmp(lexeme, "/*", 2) == 0) {
        if (strlen(lexeme) >= 4 &&
            lexeme[strlen(lexeme) - 2] == '*' &&
            lexeme[strlen(lexeme) - 1] == '/') {
            printf("<COMMENT, %s>\n", lexeme);
        } else {
            printf("<ERROR, Unterminated multi-line comment: %s>\n", lexeme);
        }
        return;
    }

    // --- Invalid ---
    printf("<ERROR, Invalid symbol: %s>\n", lexeme);
}

// -------------------- MAIN --------------------
int main() {
    char input[MAX_LEN];
    printf("Enter your program (end with a blank line):\n");

    while (fgets(input, sizeof(input), stdin)) {
        if (strcmp(input, "\n") == 0) break; // blank line ends input

        // remove newline
        input[strcspn(input, "\n")] = 0;

        // Simple space-based token split
        char *tok = strtok(input, " ");
        while (tok != NULL) {
            classifyToken(tok);
            tok = strtok(NULL, " ");
        }
    }

    return 0;
}

