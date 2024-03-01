#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_CODE_LENGTH 500

// Token types
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym,
    oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym,
    commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym,
} token_type;

// Symbol types
typedef enum {
    constant = 1, variable, procedure
} symbol_type;

// Symbol structure
typedef struct {
    char name[12];
    symbol_type kind;
    int val;
    int level;
    int addr;
} symbol;

// Instruction structure
typedef struct {
    int op; // opcode
    int l;  // L
    int m;  // M
} instruction;

// Global variables
char input[5000]; // input string
char lexeme[12]; // current lexeme
token_type token; // current token
int level = 0; // current scope level
int symbol_table_size = 0; // size of symbol table
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE]; // symbol table
instruction code[MAX_CODE_LENGTH]; // code array
int cx = 0; // code index

// Function prototypes
void parse(void);
void program(void);
void block(void);
void const_declaration(void);
void var_declaration(void);
void proc_declaration(void);
void statement(void);
void condition(void);
void expression(void);
void term(void);
void factor(void);
void emit(int op, int l, int m);
void enter(symbol_type kind);
int position(char* name);
void error(char* message);

int main(int argc, char** argv) {
    // Read input file into input string
    if (argc < 2) {
        printf("Usage: ./parser <input_file>\n");
        return 0;
    }
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Error: File not found.\n");
        return 0;
    }
    fgets(input, 5000, fp);
    fclose(fp);
    // Parse input
    parse();
    // Print symbol table
    printf("Symbol table:\n");
    printf("Name  Kind  Val  Level  Addr\n");
    for (int i = 0; i < symbol_table_size; i++) {
        printf("%s    %d    %d    %d    %d\n", symbol_table[i].name, symbol_table[i].kind, symbol_table[i].val, symbol_table[i].level, symbol_table[i].addr);
    }
    // Print code
    printf("Code:\n");
    for (int i = 0; i < cx; i++) {
        printf("%d %d %d\n", code[i].op, code[i].l, code[i].m);
    }
    return 0;
}

// Parses the input string
void parse(void) {
    // Initialize symbol table
    symbol_table[0] = (symbol){ "", 0, 0, 0, 0 };
// Parse input
getToken();
program();
if (token != periodsym) {
error("Expected period.");
}
}

// Parses program
void program(void) {
block();
if (token != periodsym) {
error("Expected period.");
}
emit(9, 0, 2); // SIO HLT
}

// Parses block
void block(void) {
level++;
int space = 4;
// Allocate space for AR and SL
emit(6, 0, space);
if (token == constsym) {
const_declaration();
}
if (token == varsym) {
var_declaration(&space);
}
while (token == procsym) {
proc_declaration();
}
// Update AR and SL
emit(7, 0, space+symbol_table_size+1);
statement();
// Restore previous level
level--;
}

// Parses constant declaration
void const_declaration(void) {
do {
getToken();
if (token != identsym) {
error("Expected identifier.");
}
getToken();
if (token != eqsym) {
error("Expected equals.");
}
getToken();
if (token != numbersym) {
error("Expected number.");
}
enter(constant);
getToken();
} while (token == commasym);
if (token != semicolonsym) {
error("Expected semicolon.");
}
getToken();
}

// Parses variable declaration
void var_declaration(int* space) {
do {
getToken();
if (token != identsym) {
error("Expected identifier.");
}
enter(variable);
(*space)++;
getToken();
} while (token == commasym);
if (token != semicolonsym) {
error("Expected semicolon.");
}
getToken();
}

// Parses procedure declaration
void proc_declaration(void) {
getToken();
if (token != identsym) {
error("Expected identifier.");
}
enter(procedure);
getToken();
if (token != semicolonsym) {
error("Expected semicolon.");
}
getToken();
block();
if (token != semicolonsym) {
error("Expected semicolon.");
}
getToken();
}

// Parses statement
void statement(void) {
int i, cx1, cx2;
switch (token) {
case identsym:
i = position(lexeme);
if (i == -1) {
error("Undeclared identifier.");
}
if (symbol_table[i].kind != variable) {
error("Assignment to constant or procedure is not allowed.");
}
getToken();
if (token != becomessym) {
error("Expected becomes.");
}
getToken();
expression();
emit(4, level-symbol_table[i].level, symbol_table[i].addr);
break;
case callsym:
getToken();
if (token != identsym) {
error("Expected identifier.");
}
i = position(lexeme);
if (i == -1) {
error("Undeclared identifier.");
}
if (symbol_table[i].kind != procedure) {
error("Call of a variable is not allowed.");
}
emit(5, level-symbol_table[i].level, symbol_table[i].addr);
getToken();
break;
case beginsym:
getToken();
statement();
while (token == semicolonsym) {
getToken();
statement();
}
if (token != endsym) {
error("Expected end.");
}
// Parses statement (continued)
getToken();
break;
case ifsym:
getToken();
condition();
if (token != thensym) {
error("Expected then.");
}
getToken();
cx1 = cx;
emit(8, 0, 0); // JPC 0, 0
statement();
code[cx1].m = cx;
if (token == elsesym) {
getToken();
cx2 = cx;
emit(7, 0, 0); // JMP 0, 0
code[cx1].m = cx;
statement();
code[cx2].m = cx;
}
break;
case whilesym:
cx1 = cx;
getToken();
condition();
cx2 = cx;
emit(8, 0, 0); // JPC 0, 0
if (token != dosym) {
error("Expected do.");
}
getToken();
statement();
emit(7, 0, cx1); // JMP 0, cx1
code[cx2].m = cx;
break;
case readsym:
getToken();
if (token != identsym) {
error("Expected identifier.");
}
i = position(lexeme);
if (i == -1) {
error("Undeclared identifier.");
}
if (symbol_table[i].kind != variable) {
error("Assignment to constant or procedure is not allowed.");
}
emit(10, 0, 2); // SIO READ
emit(4, level-symbol_table[i].level, symbol_table[i].addr);
getToken();
break;
case writesym:
getToken();
expression();
emit(9, 0, 1); // SIO WRITE
break;
default:
break;
}
}

// Parses condition
void condition(void) {
if (token == oddsym) {
getToken();
expression();
emit(2, 0, 6); // ODD
} else {
expression();
if (!(token == eqsym || token == neqsym || token == lessym || token == leqsym || token == gtrsym || token == geqsym)) {
error("Expected relational operator.");
}
token_type op = token;
getToken();
expression();
switch (op) {
case eqsym:
emit(2, 0, 8); // EQL
break;
case neqsym:
emit(2, 0, 9); // NEQ
break;
case lessym:
emit(2, 0, 10); // LSS
break;
case leqsym:
emit(2, 0, 11); // LEQ
break;
case gtrsym:
emit(2, 0, 12); // GTR
break;
case geqsym:
emit(2, 0, 13); // GEQ
break;
default:
break;
}
}
}

// Parses expression
void expression(void) {
token_type addop;
if (token == plussym || token == minussym) {
addop = token;
getToken();
term();
if (addop == minussym) {
emit(2, 0, 1); // NEG
}
} else {
term();
}
while (token == plussym || token == minussym) {
addop = token;
getToken();
term();
if (addop == plussym) {
emit(2, 0, 2); // ADD
} else {
emit(2, 0, 3); // SUB
}
}
}

// Parses term
void term(void) {
token_type mulop;
factor();
while (token == multsym || token == slashsym) {
mulop = token;
getToken();
factor();
if (mulop == multsym) {
emit(2, 0, 4); //// Parses term (continued)
} else {
emit(2, 0, 5); // DIV
}
}
}

// Parses factor
void factor(void) {
int i;
switch (token) {
case identsym:
i = position(lexeme);
if (i == -1) {
error("Undeclared identifier.");
}
switch (symbol_table[i].kind) {
case constant:
emit(1, 0, symbol_table[i].val);
break;
case variable:
emit(3, level-symbol_table[i].level, symbol_table[i].addr);
break;
case procedure:
error("Call of a procedure is not allowed.");
break;
}
getToken();
break;
case numbersym:
emit(1, 0, atoi(lexeme));
getToken();
break;
case lparentsym:
getToken();
expression();
if (token != rparentsym) {
error("Expected right parenthesis.");
}
getToken();
break;
default:
error("Invalid factor.");
break;
}
}

// Emits an instruction
void emit(int op, int l, int m) {
if (cx >= MAX_CODE_LENGTH) {
error("Program too long.");
}
code[cx].op = op;
code[cx].l = l;
code[cx].m = m;
cx++;
}

// Adds a symbol to the symbol table
void enter(symbol_type kind) {
if (symbol_table_size >= MAX_SYMBOL_TABLE_SIZE) {
error("Symbol table full.");
}
strcpy(symbol_table[symbol_table_size].name, lexeme);
symbol_table[symbol_table_size].kind = kind;
symbol_table[symbol_table_size].level = level;
switch (kind) {
case constant:
symbol_table[symbol_table_size].val = atoi(lexeme);
break;
case variable:
symbol_table[symbol_table_size].addr = 4 + symbol_table_size;
break;
case procedure:
symbol_table[symbol_table_size].addr = cx;
break;
}
symbol_table_size++;
}

// Returns the position of a symbol in the symbol table
int position(char* name) {
for (int i = symbol_table_size-1; i >= 0; i--) {
if (strcmp(symbol_table[i].name, name) == 0) {
return i;
}
}
return -1;
}

// Prints an error message and exits
void error(char* message) {
printf("Error: %s\n", message);
exit(1);
}