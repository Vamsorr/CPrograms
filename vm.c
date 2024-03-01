/* Sam Voor
* Carter Susi
* COP3402 Spring 2023
* Homework 1: P-Machine
*/
/*
 
 compile:
            gcc -o vm vm.c
            ./vm input.txt
 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define ARRAY_SIZE 500

int pas[ARRAY_SIZE];

//instructions
typedef struct instruction {
    int OP; //operation
    int L;  //lexicographical
    int M;  //number
}instruction;

//registers
typedef struct PM_0{
    int BP;         //base pointer
    int SP;         //stack pointer
    int PC;         //program counter
    instruction IR; //instruction Register
}PM_0;

int base( int BP, int L){
    
    int arb = BP;   // arb = activation record base
    while ( L > 0){ //find base L levels down
        arb = pas[arb];
        L--;
    }
    return arb;
}

struct PM_0 CPU;
struct instruction TEXT;
int main(int argc, const char * argv[]) {
    
    
//create intital values--------------------------------------------------------------------------------------------------
    CPU.BP = 499;
    CPU.SP = CPU.BP+1;
    CPU.PC = 0;
    for(int x = 0; x<ARRAY_SIZE;x++){
        pas[x] = 0;
    }
    int TEXT[3];
    int init_SP = CPU.SP;   //to remember the initial SP
    int init_BP = CPU.BP;   //to remember the initial BP
    bool AR = false;        //if activation record is made, set to true
    printf("\t\t\t\t\tPC\tBP\tSP\tstack\n");
    printf("Initital Values:\t%d\t%d\t%d\n",CPU.PC,CPU.BP,CPU.SP);
//--------------------------------------------------------------------------------------------------------------------
    
    
//gather input--------------------------------------------------------------------------------------------------------
    FILE* input;
    input = fopen(argv[1], "r");//change to terminal input after testing
    
    if(input == NULL){
        fprintf(stderr, "File not found... ending program.\n");
        return 0;
    }
    else{
        int i = 0;//  i = the number of input
        while (!feof(input)){
            fscanf(input,"%d%d%d", &pas[i], &pas[i+1], &pas[i+2]);
            i+=3;
        }
    }
//--------------------------------------------------------------------------------------------------------------------

    
//creating the TEXT array to represent the current instruction-------------------------------------------------------
    TEXT[0] = pas[0];
    TEXT[1] = pas[1];
    TEXT[2] = pas[2];
//--------------------------------------------------------------------------------------------------------------------

    
//while loop until EOP------------------------------------------------------------------------------------------------
    
    do{
        switch(TEXT[0]){
//--------------------------------------------------------------------------------------------------------------------------
//case 1 (LIT),
//--------------------------------------------------------------------------------------------------------------------------
            case(1):
                CPU.SP -= 1;
                pas[CPU.SP] = TEXT[2];
                printf("\t\tLIT\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                break;
//--------------------------------------------------------------------------------------------------------------------------
//case 2 (RTN),
//--------------------------------------------------------------------------------------------------------------------------
            case(2):
                switch (TEXT[2]) {
                    case(0):
                        if(TEXT[1] == 0){
                            CPU.SP = CPU.BP + 1;
                            CPU.BP = pas[CPU.SP-2];
                            CPU.PC = pas[CPU.SP-3];
                            AR = false;
                        }
                        break;
                // ADD
                case (1):
                  pas[CPU.SP++] = pas[CPU.SP++] + pas[CPU.SP];
                  CPU.SP++;
                  break;
                // SUB
                case (2):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] - pas[CPU.SP];
                  CPU.SP++;
                  break;
                  // MUL
                case (3):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] * pas[CPU.SP];
                  CPU.SP++;
                  break;
                  // DIV
                case (4):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] / pas[CPU.SP];
                  CPU.SP++;
                  break;
                  // EQL
                case (5):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] == pas[CPU.SP];
                  CPU.SP++;
                  break;
                  // NEQ
                case (6):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] != pas[CPU.SP];
                  CPU.SP++;
                  break;
                  // LSS
                case (7):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] < pas[CPU.SP];
                  CPU.SP++;
                  break;
                  // LEQ
                case (8):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] <= pas[CPU.SP];
                  CPU.SP++;
                  break;
                  // GTR
                case (9):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] > pas[CPU.SP];
                  CPU.SP++;
                  // GEQ
                case (10):
                  pas[CPU.SP + 1] = pas[CPU.SP + 1] >= pas[CPU.SP];
                  CPU.SP++;
                  break;
                }
                printf("\t\tRTN\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                break;
//--------------------------------------------------------------------------------------------------------------------------
//case 3 (LOD),
//--------------------------------------------------------------------------------------------------------------------------
            case(3):
                CPU.SP-=1;
                pas[CPU.SP] = pas[base(CPU.BP, TEXT[1]) - TEXT[2]];
                printf("\t\tLOD\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                break;
//--------------------------------------------------------------------------------------------------------------------------
//case 4 (STO),
//--------------------------------------------------------------------------------------------------------------------------
            case(4):
                pas[base(CPU.BP, TEXT[1]) - TEXT[2]] = pas[CPU.SP];
                CPU.SP++;
                printf("\t\tSTO\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                break;
//--------------------------------------------------------------------------------------------------------------------------
//case 5 (CAL),
//--------------------------------------------------------------------------------------------------------------------------
            case(5):
                AR = true;
                CPU.BP = CPU.SP - 1;
                pas[CPU.SP-1] = init_SP - 1;
                pas[CPU.SP-2] = init_BP;
                pas[CPU.SP-3] = CPU.PC;
                CPU.PC = TEXT[2];
                printf("\t\tCAL\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                break;
//--------------------------------------------------------------------------------------------------------------------------
//case 6 (INC), subtracts M number of locals from SP, when stack output is printed it is printed from SP until original SP
//--------------------------------------------------------------------------------------------------------------------------
            case(6):
                CPU.SP = CPU.SP - TEXT[2];
                printf("\t\tINC\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                break;
//--------------------------------------------------------------------------------------------------------------------------
//case 7 (JMP), changes TEXT struct to the specified PC Counter (TEXT[2])
//--------------------------------------------------------------------------------------------------------------------------
            case(7):
                CPU.PC = TEXT[2];
                printf("\t\tJMP\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                break;
//--------------------------------------------------------------------------------------------------------------------------
//case 8 (JPC),
//--------------------------------------------------------------------------------------------------------------------------
            case(8):
                if (pas[CPU.SP] == 0) {
                  CPU.PC = TEXT[2];
                  CPU.SP++;
                }
                printf("\t\tJPC\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                break;
//--------------------------------------------------------------------------------------------------------------------------
//case 9 (SYS),
//--------------------------------------------------------------------------------------------------------------------------
            case(9):
                switch(TEXT[2]){
                    case(1):
                        printf("Output result is: %d\n",pas[CPU.SP]);
                        CPU.SP += TEXT[2];
                        printf("\t\tSOU\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                        break;
                    case(2):
                        CPU.SP = CPU.SP - 1;
                        int user_in;
                        while(1){
                            printf("Please Enter an integer: ");
                            if(scanf("%d", &user_in)==1){
                                pas[CPU.SP] = user_in;
                                break;
                            }else{
                                printf("Invalid input...\n");
                                scanf("%*s");
                            }
                        }
                        printf("\t\tSIN\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
                        break;
                    case(3):
                        break;
                }
                break;
//--------------------------------------------------------------------------------------------------------------------------
//
        //end of case statements
        }
        
//--------------------------------------stores and displays next instruction --------------------------------------------
        
//display the next instruction---------------------------------------------------------------------------------------
        
//if the SP has changed, display the stack---------------------------------------------------------------------------
        
        if(CPU.SP != init_SP){
            for(int y = init_SP-1 ; y >= CPU.SP ; y--){
                if(y == CPU.BP && AR == true){
                    printf("|\t");
                }
                printf("%d\t", pas[y]);
            }
        }
        
        printf("\n");
//--------------------------------------------------------------------------------------------------------------------
        TEXT[0] = pas[CPU.PC];
        TEXT[1] = pas[CPU.PC+ 1];
        TEXT[2] = pas[CPU.PC + 2];
        CPU.PC+=3;
        
//--------------------------------------------------------------------------------------------------------------------
    //end of while loop
    }while(!(TEXT[0] == 9 && TEXT[1] == 0 && TEXT[2] == 3));
    
//print EOP statement-------------------------------------------------------------------------------------------------
    printf("\t\tEOP\t%d\t%d\t%d\t%d\t%d\t", TEXT[1],TEXT[2],CPU.PC,CPU.BP,CPU.SP);
    if(CPU.SP != init_SP){
        for(int y = init_SP-1 ; y >= CPU.SP ; y--){
            if(y == CPU.BP && AR == true){
                printf("|\t");
            }
            printf("%d\t", pas[y]);
        }
    }
printf("\n");
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
    return 0;
 
}
