/*  Sam Voor
    Carter Susi
    COP3402 Spring 2023
    Programming Assignment 4
*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXIDLEN 11   //Identigiers
#define MAXINTLEN 5     //Numbers
#define MAX_SYMBOL_TABLE_SIZE 500


//enums for token type------------------------------------------------------------
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym,
    oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym,
    commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym
} token_type;
//--------------------------------------------------------------------------------

//enums for OP--------------------------------------------------------------------
typedef enum {
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SYS

} OPs;
//--------------------------------------------------------------------------------

//enums for operands--------------------------------------------------------------
typedef enum {
    ADD = 1, SUB, MUL, DIV, EQL, NEQ, LSS, LEQ, GTR, GEQ
} OPRs;
//--------------------------------------------------------------------------------

//structs forassembly-------------------------------------------------
typedef struct instruction {
    int line;
    char* OP; //operation
    int L;  //lexicographical
    int M;  //number
}instruction;
//creating arrays to store instruction structs
instruction instruction_table[MAX_SYMBOL_TABLE_SIZE];
static int it_count = 0;
//--------------------------------------------------------------------------------


//helpers to add instructiond-----------------------------------------------------
instruction create_instruction(int line, char *OP, int L, int M) {
    instruction new_instruction;

    new_instruction.line = line;
    new_instruction.OP = malloc(strlen(OP) + 1);
    strcpy(new_instruction.OP, OP);
    new_instruction.L = L;
    new_instruction.M = M;

    return new_instruction;
}
//--------------------------------------------------------------------------------


//static variables----------------------------------------------------------------
//static int line_num = 0;
static int lvl = 0;
//static int M_addr = 3;

static int operands[] = {4, 5, 6, 7, 9, 10, 11, 12, 13, 14};
//--------------------------------------------------------------------------------


//struct to pass into program, stores the string and token type-------------------
typedef struct {
    char name[50];
    token_type type;
    int levl;
    int symbol;
} Token;
//--------------------------------------------------------------------------------


//functions-----------------------------------------------------------------------
static int token_index = 0;         //to index tokenList throughout functions
void fix_JMP(int index, int new_m); //sets a new M for JMP
void error(int error_code);         //prints correlating error message
void get();                         //increments index
void block(Token *tokenList);       //starts block of instructions
void program(Token *tokenList);     //starts and ends recursion
void statement(Token *tokenList);
void expression(Token *tokenList);
void condition(Token *tokenList);
void term(Token *tokenList);
void factor(Token *tokenList);

//--------------------------------------------------------------------------------

void fix_JMP(int index, int new_m){
    /*int flag = 0
    for(int i = 0; i< it_count; i++){
        if(instruction_table[i].OP == JMP && instruction_table[i].M == 0){
            flag = i;
        }
    }*/
    instruction_table[index].M = new_m;
}

//error func call-----------------------------------------------------------------
//remove ' at index: %d, ' and token_index var before sumbitting, to help debug
void error(int error_code){
    switch (error_code) {
        case 1:
            printf("Error number %d, at index: %d,  Use = instead of :=.\n",error_code, token_index);
            break;
        case 2:
            printf("Error number %d, at index: %d,  = must be followed by a number.\n",error_code, token_index);
            break;
        case 3:
            printf("Error number %d, at index: %d,  Identifier must be followed by =.\n",error_code, token_index);
            break;
        case 4:
            printf("Error number %d, at index: %d,  const, var, procedure must be followed by identifier.\n",error_code, token_index);
            break;
        case 5:
            printf("Error number %d, at index: %d,  Semicolon or comma missing.\n",error_code, token_index);
            break;
        case 6:
            printf("Error number %d, at index: %d,  Incorrect symbol after procedure declaration.\n",error_code, token_index);
            break;
        case 7:
            printf("Error number %d, at index: %d,  Statement expected.\n",error_code, token_index);
            break;
        case 8:
            printf("Error number %d, at index: %d,  Incorrect symbol after statement part in block.\n",error_code, token_index);
            break;
        case 9:
            printf("Error number %d, at index: %d,  Period expected.\n",error_code, token_index);
            break;
        case 10:
            printf("Error number %d, at index: %d,  Semicolon between statements missing.\n",error_code, token_index);
            break;
        case 11:
            printf("Error number %d, at index: %d,  Undeclared identifier.\n",error_code, token_index);
            break;
        case 12:
            printf("Error number %d, at index: %d,  Assignment to constant or procedure is not allowed.\n",error_code, token_index);
            break;
        case 13:
            printf("Error number %d, at index: %d,  Assignment operator expected.\n",error_code, token_index);
            break;
        case 14:
            printf("Error number %d, at index: %d,  call must be followed by an identifier.\n",error_code, token_index);
            break;
        case 15:
            printf("Error number %d, at index: %d,  Call of a constant or variable is meaningless.\n",error_code, token_index);
            break;
        case 16:
            printf("Error number %d, at index: %d,  then expected.\n",error_code, token_index);
            break;
        case 17:
            printf("Error number %d, at index: %d,  Semicolon or end expected.\n",error_code, token_index);
            break;
        case 18:
            printf("Error number %d , at index: %d,  do expected.\n",error_code, token_index);
            break;
        case 19:
            printf("Error number %d, at index: %d,  Incorrect symbol following statement.\n",error_code, token_index);
            break;
        case 20:
            printf("Error number %d, at index: %d,  Relational operator expected.\n",error_code, token_index);
            break;
        case 21:
            printf("Error number %d, at index: %d,  Expression must not contain a procedure identifier.\n",error_code, token_index);
            break;
        case 22:
            printf("Error number %d, at index: %d,  Right parenthesis missing.\n",error_code, token_index);
            break;
        case 23:
            printf("Error number %d, at index: %d,  The preceding factor cannot begin with this symbol.\n",error_code, token_index);
            break;
        case 24:
            printf("Error number %d, at index: %d,  An expression cannot begin with this symbol.\n",error_code, token_index);
            break;
        case 25:
            printf("Error number %d, at index: %d,  This number is too large.\n",error_code, token_index);
            break;
        case 26:
            printf("Error number %d, at index: %d,  Identifier too long.\n",error_code, token_index);
            break;
        case 27:
            printf("Error number %d, at index: %d,  Invalid symbol.\n",error_code, token_index);
            break;
        default:
            printf("Invalid error code %d, at index: %d, \n",error_code, token_index);
    }
    exit(1);
}

//--------------------------------------------------------------------------------


//get func call-------------------------------------------------------------------
static bool flag = false;//needed to return the first varaible 0
void get(){
    if(flag == true){
        token_index++;
    }
    else{
        flag = true;
    }
}
//--------------------------------------------------------------------------------
int find_token(Token *tokenList, char* token){
    for(int i = 0; i< token_index;i++){
        if(strcmp(token,tokenList[i].name) == 0){
            return i;
        }
    }
    return 0;
}
int find_sym(Token * tokenList, int place){
    for (int i = place; i >= 0 ;i--){
        if(tokenList[i].type == procsym || tokenList[i].type == varsym || tokenList[i].type == constsym){
            return i;
        }
    }
    return 0;
}

//block func call-----------------------------------------------------------------
void block(Token *tokenList) {
    lvl++;
    //printf("%d",lvl);
    int space = 4;
    int jmpaddr = it_count;
    instruction_table[it_count] = create_instruction(it_count,"JMP", 0, 0);
    it_count++;
    //constsym------------------------------------------------------------------------
    if (tokenList[token_index].type == constsym){
        tokenList[token_index].levl = lvl;
        do {
            get();
            if (tokenList[token_index].type != identsym){
                    error(4);
            }
            get();
            if (tokenList[token_index].type != eqsym){
                    error(3);
            }
            get();
            if (tokenList[token_index].type != numbersym){
                    error(2);
            }
            get();
        } while(tokenList[token_index].type == commasym);
        if (tokenList[token_index].type != semicolonsym){
                error(17);
        }
        get();
    }
    //--------------------------------------------------------------------------------

    //varsym--------------------------------------------------------------------------
    if (tokenList[token_index].type == varsym) {
        tokenList[token_index].levl = lvl;
        do {
            get();
            if (tokenList[token_index].type != identsym){
                    error(4);
            }
            tokenList[token_index].levl = lvl;
            get();
            space++;
        } while(tokenList[token_index].type == commasym);
        if (tokenList[token_index].type != semicolonsym){
                error(17);
        }
        get();
    }
    //--------------------------------------------------------------------------------

    //procsym-------------------------------------------------------------------------
    while (tokenList[token_index].type == procsym) {
        tokenList[token_index].levl = lvl;
        get();
        if (tokenList[token_index].type != identsym){
                error(4);
        }
        get();
        if (tokenList[token_index].type != semicolonsym){
                error(17);
        }
        get();
        block(tokenList);
        if (tokenList[token_index].type != semicolonsym){
                error(17);
        }
        get();
    }
    fix_JMP(jmpaddr,it_count*3);
    instruction_table[it_count] = create_instruction(it_count,"INC",0,space);
    it_count++;
    statement(tokenList);
    instruction_table[it_count] = create_instruction(it_count,"RTN",0,0);
    it_count++;
    lvl--;
    //--------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------


//program func call---------------------------------------------------------------
void program(Token *tokenList) {
    get();
    block(tokenList);
    if (tokenList[token_index].type != periodsym){
            error(9);
    }
    instruction_table[it_count] = create_instruction(it_count,"EOP",0,3);
    it_count++;
}
//statement func call--------------------------------------------------------------------------------
void statement(Token *tokenList) {
    if (tokenList[token_index].type == identsym) {
        int temp = find_token(tokenList, tokenList[token_index].name);
        //printf("%d",temp);
        int sym = find_sym(tokenList,temp);
        //printf("%d",sym);
        if(tokenList[sym].type != varsym){
            error(11);
        }
        if(tokenList[sym].type == constsym || tokenList[sym].type == procsym){
            error(12);
        }
        int ident_lvl = tokenList[sym].levl;
        get();
        if (tokenList[token_index].type == becomessym) {
            get();
            expression(tokenList);
            instruction_table[it_count] = create_instruction(it_count,"STO",ident_lvl,temp);
            it_count++;
        } else {
            error(13);
        }
    }
    else if (tokenList[token_index].type == callsym) {
        get();
        if (tokenList[token_index].type == identsym) {
            int temp = find_token(tokenList,tokenList[token_index].name);
            if(tokenList[temp - 1].type == procsym){
                int proc_lvl = tokenList[temp-1].levl;
                instruction_table[it_count] = create_instruction(it_count,"CAL",lvl-proc_lvl,temp);
                it_count++;
            }
            get();
        } else {
            error(14);
        }
    }
    else if (tokenList[token_index].type == beginsym) {
        get();
        statement(tokenList);
        while (tokenList[token_index].type == semicolonsym){
            get();
            if(tokenList[token_index].type == endsym){
                break;
            }
            statement(tokenList);
        }
        if (tokenList[token_index].type != endsym) {
                error(17);
        }
        get();
    }
    else if (tokenList[token_index].type == ifsym) {
        get();
        condition(tokenList);
        int temp_jpc = it_count;
        instruction_table[it_count] = create_instruction(it_count, "JPC", 0, 0);
        it_count++;
        if (tokenList[token_index].type == thensym) {
            get();
            statement(tokenList);
            instruction_table[temp_jpc].M = it_count*3;
        } else {
            error(16);
        }
    }
    else if (tokenList[token_index].type == whilesym) {
        int temp_jmp = it_count;
        get();
        condition(tokenList);
        int temp_jpc = it_count;
        instruction_table[it_count] = create_instruction(it_count, "JPC", 0, 0);
        it_count++;
        if (tokenList[token_index].type == dosym) {
            get();
            statement(tokenList);
            instruction_table[it_count] = create_instruction(it_count, "JMP", 0, temp_jmp);
            it_count++;
            instruction_table[temp_jpc].M = it_count;
        } else {
            error(18);
        }
    }
    else if (tokenList[token_index].type == writesym) {
        get();
        expression(tokenList);
    }
    else {
        error(21);
    }
}
//expression func call--------------------------------------------------------------------------------
void expression(Token *tokenList) {
    if (tokenList[token_index].type == plussym || tokenList[token_index].type == minussym) {
        for(int i = 0; i<10;i++){
                if(tokenList[token_index].type == operands[i]){
                    instruction_table[it_count] = create_instruction(it_count,"OPR",0,i+1);
                    it_count++;
                }
            }
        get();
    }
    term(tokenList);
    while (tokenList[token_index].type == plussym || tokenList[token_index].type == minussym) {
        for(int i = 0; i<10;i++){
                if(tokenList[token_index].type == operands[i]){
                    instruction_table[it_count] = create_instruction(it_count,"OPR",0,i+1);
                    it_count++;
                }
            }
        get();
        term(tokenList);
    }
}
//condition func call--------------------------------------------------------------------------------
void condition(Token *tokenList) {
    if (tokenList[token_index].type == oddsym) {
        get();
        expression(tokenList);
    }
    else {
        expression(tokenList);
        if (tokenList[token_index].type <= eqsym && tokenList[token_index].type >= geqsym) {
            error(20);
        }
        else {
            for(int i = 0; i<10;i++){
                if(tokenList[token_index].type == operands[i]){
                    instruction_table[it_count] = create_instruction(it_count,"OPR",0,i+1);
                    it_count++;
                }
            }
            get();
            expression(tokenList);
        }
    }
}
//term func call--------------------------------------------------------------------------------
void term(Token *tokenList) {
    factor(tokenList);
    while (tokenList[token_index].type == multsym || tokenList[token_index].type == slashsym) {
        for(int i = 0; i<10;i++){
                if(tokenList[token_index].type == operands[i]){
                    instruction_table[it_count] = create_instruction(it_count,"OPR",0,i+1);
                    it_count++;
                }
            }
        get();
        factor(tokenList);
    }
}
//factor func call--------------------------------------------------------------------------------
void factor(Token *tokenList) {
    if (tokenList[token_index].type == identsym) {
        int temp = find_token(tokenList, tokenList[token_index].name);
        int temp_sym = find_sym(tokenList, temp);
        //printf("%d %s %d\n%s %d\n\n",token_index, tokenList[temp].name,temp,tokenList[temp_sym].name,temp_sym);
        if(tokenList[temp_sym].type == procsym || temp == 0){
            error(21);
        }
        if(tokenList[temp_sym].type == varsym){
            instruction_table[it_count] = create_instruction(it_count,"LOD",tokenList[temp_sym].levl,temp);
            it_count++;
        }
        get();
    } else if (tokenList[token_index].type == numbersym) {
        instruction_table[it_count] = create_instruction(it_count,"LIT",0,atoi(tokenList[token_index].name));
        it_count++;
        get();
    } else if (tokenList[token_index].type == lparentsym) {
        get();
        expression(tokenList);
        if (tokenList[token_index].type != rparentsym) {
            error(22);
        }
        get();
    } else {
        error(23);
    }
}

void add_token(char *buf, int length, char **tokens, int *token_count) {
    tokens[*token_count] = malloc(length + 1);
    strncpy(tokens[*token_count], buf, length);
    tokens[*token_count][length] = '\0';
    (*token_count)++;
}

void cleanArray(char**tokens, int *token_count) {
    for (int i = 0; i < *token_count; ++i) {
        bool onlyWhitespace = true;

        for (int j = 0; j < strlen(tokens[i]); ++j) {
            if (!isspace(tokens[i][j])) {
                onlyWhitespace = false;
                break;
            }
        }

        if (onlyWhitespace) {
            for (int j = i; j < *token_count - 1; ++j) {
                strcpy(tokens[j], tokens[j + 1]);
            }
            (*token_count)--;
            i--;
        }
    }

    for (int i = 0; i < *token_count; ++i) {
        int start = 0;
        int end = strlen(tokens[i]) - 1;

        while (start <= end && isspace(tokens[i][start])) {
            start++;
        }

        while (end >= start && isspace(tokens[i][end])) {
            end--;
        }

        for (int j = start, k = 0; j <= end; ++j, ++k) {
            tokens[i][k] = tokens[i][j];
        }
        tokens[i][end - start + 1] = '\0';
    }
}


int main() {
    FILE *file;
    file = fopen("input2.txt", "r");
    char* source_program = malloc(sizeof(char) * (MAX_SYMBOL_TABLE_SIZE * MAXIDLEN));
    int sp_count = 0;
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char c;
    int size = 0;
    while ((c = fgetc(file)) != EOF) {
        source_program[sp_count] = c;
        sp_count++;
        if (!isspace(c)) {
            size++;
        }
    }

    fseek(file, 0, SEEK_SET);

    char **tokens = malloc(sizeof(char *) * size);
    int token_count = 0;
    char buf[128];
    int buf_index = 0;

     while ((c = fgetc(file)) != EOF) {
        if (isalnum(c) || c == '_') {
            buf[buf_index++] = c;
        } else {
            if (buf_index > 0) {
                add_token(buf, buf_index, tokens, &token_count);
                buf_index = 0;
            }
            if (c == ':' && (c = fgetc(file)) == '=') {
                buf[buf_index++] = ':';
                buf[buf_index++] = '=';
            } else {
                buf[buf_index++] = c;
            }
            if (!isspace(c)) {
                add_token(buf, buf_index, tokens, &token_count);
                buf_index = 0;
            }
        }
    }

    if (buf_index > 0) {
        add_token(buf, buf_index, tokens, &token_count);
        buf_index = 0;
    }

    fclose(file);

    cleanArray(tokens, &token_count);
    //--------------------------------------------------------------------------------


/*variables for output------------------------------------------------------------------------------------------*/

    //printf("Lexeme Table:\n\nlexeme\t\ttoken type\n");
    char *token_names[] = {"null", "skip", "ID", "NUM", "+", "-", "*", "/", "odd", "=", "<>", "<", "<=", ">", ">=", "lparent", "rparent", ",", ";", ".", ":=", "begin", "end", "if", "then", "while", "do", "call", "const", "var", "procedure", "write", "read", "else"};
    int num_names = sizeof(token_names) / sizeof(token_names[0]);

    //variables for lex list
    int num_in_out= sizeof(token_names) * 2;
    int type_tokens[num_in_out];
    char str_out[num_in_out][50];
    int i_counter = 0;
    int s_counter = 0;

/*find token type-----------------------------------------------------------------------------------------------*/
    int halt = 0; //stop if comment
    for(int y = 0; y < token_count ; y++){

        //checking for comments
        if(strcmp(tokens[y],"/*") == 0){
            halt = 1;
        }
        if(strcmp(tokens[y],"*/") == 0){
            halt = 0;
            continue;
        }

        int type = 0;//output for correlating type name

        //if not a comment, find the token type
        if(halt == 0){
            //checking if token has a predefined name
            for(int x = 0; x < num_names; x++){
                if(strcmp(tokens[y], token_names[x]) == 0){
                    type = x;
                }
            }

            //if the token does not have a predefined type, find it
            if(type == 0){
                int len = strlen(tokens[y]);
                if(isalpha(tokens[y][0])){
                    if(len > MAXIDLEN){
                        error(26);
                        continue;
                    }
                    type = 2;
                }
                else if(isdigit(tokens[y][0])){
                    if(len > MAXINTLEN){
                        error(25);
                        continue;
                    }
                    char *endptr;
                    long value = strtol(tokens[y], &endptr, 10);
                    if (*endptr != '\0') {
                       error(24);
                        continue;
                    }
                    type = 3;
                }
                else{
                    error(27);
                }
            }

            //proper spacing in output
            if(strlen(tokens[y]) > 8){
                //printf("%s\t%d\t%d\n",tokens[y], type, y);
                type_tokens[++i_counter] = type;
                if(type == 2){
                    strcpy(str_out[++s_counter], tokens[y]);

                }
            }
            else{
                //printf("%s\t\t%d\t\t%d\n",tokens[y], type, y);
                type_tokens[++i_counter] = type;
                if(type == 2 || type == 3){
                    strcpy(str_out[++s_counter], tokens[y]);
                }
            }
        }
    }

    //creating token struct array-----------------------------------------------------
    Token tokenList[token_count];

    for (int i = 0; i < token_count; i++) {
        strcpy(tokenList[i].name, tokens[i]);
        tokenList[i].type = type_tokens[i+1];
        tokenList[i].levl = 0;
        //printf("%d\t%s\t%d\n",i,tokens[i],type_tokens[i+1]);
        //instruction_table[i] = create_instruction(tokenList[i].name, tokenList[i].type, 0);
    }
    //--------------------------------------------------------------------------------

    //starting recursive descent parser-----------------------------------------------
    program(tokenList);

    printf("No errors, program is syntactically correct\n\n");
    printf("Source Program:\n\n");
    for(int i = 0; i< sp_count;i++){
        printf("%c",source_program[i]);
    }
    printf("\n\nLine\tOP\tL\tM\n");

    //print to file 'elf.txt'
    for (int i = 0; i < it_count; i++) {
        printf("%d\t%s\t%d\t%d\n",instruction_table[i].line, instruction_table[i].OP,instruction_table[i].L,instruction_table[i].M);
    }
    //--------------------------------------------------------------------------------

    /*printing assembly/symbol tables-------------------------------------------------
    assembly_table[line_num] = create_assembly(line_num, "JMP", L, M);
    line_num++;
    symbol_table[symbol_count] = create_symbol(3, "main", value, level, address, mark);
    symbol_count++;

    printf("Assembly Code:\n\nLine    OP    L    M\n");
    for(int u = 0; u < line_num; u++){
        printf("%3d %6s %4d %4d\n",assembly_table[u].Line,assembly_table[u].OP,assembly_table[u].L,assembly_table[u].M);
    }
    printf("\nSymbol Table:\n\nKind | Name        | Value | Level | Address | Mark\n---------------------------------------------------\n");
    for(int u = 0; u < symbol_count; u++){
        printf("  %d  | %11s | %-5d | %-5d | %-7d | %-4d\n",symbol_table[u].kind,symbol_table[u].name,symbol_table[u].val,symbol_table[u].level,symbol_table[u].addr,symbol_table[u].mark);
    }
    //--------------------------------------------------------------------------------*/

    return 0;

}
