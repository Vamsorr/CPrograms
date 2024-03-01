/*  Sam Voor
    Carter Susi
    COP3402 Fall 2022
    Programming Assignment 2
*/
/*
1.- [ ] means an optional item,
2.- { } means repeat 0 or more times.
3.- Terminal symbols are enclosed in quote marks.
4.- Symbols without quotes are called not a syntactic class.
5.-A period is used to indicate the end of the definition of a syntactic class.
6.-The  symbol  ‘::=’  is  read  as  ‘is  defined  as’;  for  example,  the  following  syntactic
class:


Detect the Following Lexical Errors:
1. Identigiers cannot begin with a digit.
2. Number too long.
3. Name too long.
4. Invalid symbols.
5. The input ends during a comment (i.e. a comment was started but not
ginished when an end-of-gile was seen reading from the input gile).


*/


#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXTXTSTRLEN 128
#define MAXIDLEN 11   //Identigiers can be a maximum of 11 characters in length.
#define MAXINTLEN 5     //Numbers can be a maximum of 5 digits in length.


char *tokens[MAXTXTSTRLEN];
int token_count = 0;
/*Token Types--------------------------------------------------------------------------------------------------*/
typedef enum{ skipsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,  slashsym = 7, oddsym = 8,  eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33 } token_type;
/*--------------------------------------------------------------------------------------------------------------*/



/*boolean helpers-----------------------------------------------------------------------------------------------*/
bool is_digit(char ch){
    if(ch == 0 || ch == 1 || ch == 2 || ch == 3 || ch == 4 || ch == 5 || ch == 6 || ch == 7 || ch == 8 || ch == 9){
        return true;
    }
    return false;
}

bool valid_indentifier(char* string){
    if(strlen(string) >= 11){
        return false;
    }
    if(is_digit(string[0]) == true){
        return false;
    }
    return true;
}
/*----------------------------------------------------------------------------------------------------------------*/
bool is_it(const char *token, const char *delimiters) {
    size_t i, j;
    for (i = 0; token[i]; i++) {
        for (j = 0; delimiters[j]; j++) {
            if (token[i] == delimiters[j]) {
                return true;
            }
        }
    }
    return false;
}
/*main------------------------------------------------------------------------------------------------------------*/
int main(void) {

    FILE* input;
    input = fopen("input.txt", "r");//change to terminal input after testing

/*Getting input--------------------------------------------------------------------------------------------------*/
    char **strings = NULL;
    int string_count = 0;
    char string[MAXTXTSTRLEN];
    int max_tokens = 0;

    if(input == NULL){
        fprintf(stderr, "File not found... ending program.\n");
        return 0;
    }
    else{
        while (fgets(string, MAXTXTSTRLEN, input) != NULL) {  //itterate through file to gather strings

    /*reallocate memory in 'strings'(double ptr) for next 'string'------------------------------------------------*/
            strings = (char **) realloc(strings, (string_count + 1) * sizeof(char *));
            if (strings == NULL) {
                fprintf(stderr, "memory error\n");
                fclose(input);
                return 1;
            }

    /*allocate memory for individual 'string' or char array-------------------------------------------------------*/
            strings[string_count] = (char *) malloc(strlen(string) + 1);
            if (strings[string_count] == NULL) {
                fprintf(stderr, "memory error\n");
                fclose(input);
                return 1;
            }
    /*-------------------------------------------------------------------------------------------------------------*/

            strcpy(strings[string_count], string);//storing into array
            string_count++;
        }

    /*printing initial output--------------------------------------------------------------------------------------*/
        printf("Source Program:\n");
        for (int i = 0; i < string_count; i++) {
            printf("%s", strings[i]);
            max_tokens += strlen(strings[i]);
        }
        printf("\n\nLexeme Table:\n\n");
        printf("lexeme\t\ttoken type\n");
    /*-------------------------------------------------------------------------------------------------------------*/

    }//else() end
/*-----------------------------------------------------------------------------------------------------------------*/



/*creating token array---------------------------------------------------------------------------------------------*/
    int counter = 0;
    char ** tokens = (char**) malloc(max_tokens * sizeof(char*));

    for (int i = 0; i < string_count; i++) {
        char *start = strings[i];
        char *end = strings[i] + strlen(strings[i]);
        char *token;

        while (start < end) {
            token = strpbrk(start, " \t\n,/*+;");
            if (token == NULL) {
                token = end;
            }
            int len = token - start + 1;
            char *new_token = (char*) malloc((len + 1) * sizeof(char));
            if (new_token == NULL) {
                printf("Error allocating memory\n");
                exit(1);
            }
            strncpy(new_token, start, len);
            new_token[len] = '\0';

            if(is_it(new_token,"+,*;/")) {
                char temp[2] = {new_token[len-1], '\0'};
                new_token[len -1] = '\0';
                tokens[counter++] = new_token;
                tokens[counter++] = temp;

            }
            else {
                tokens[counter++] = new_token;
            }
            start = token + 1;
        }
    }

    for (int i = 0; i < counter; i++) {
        printf(" %s\n", tokens[i]);
    }


    for (int i = 0; i < max_tokens; i++) {
        free(tokens[i]);
    }
    free(tokens);


/*-------------------------------------------------------------------------------------------------------------*/


//If the token contains a ';', ',', '*', or '/' the token is split into the rest of the string without the specified characters and the specified character.







/*freeing memory-----------------------------------------------------------------------------------------------*/
    fclose(input);
    for (int i = 0; i < string_count; i++) {
        free(strings[i]);
    }
    free(strings);

/*-------------------------------------------------------------------------------------------------------------*/
    return 0;
}

