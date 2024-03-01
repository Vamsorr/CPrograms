#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 10

int count = 0;
int stack[MAX_SIZE];


// push function
void push(int x)
{
    stack[count] = x;
    count ++;
}

// pop function
int pop()
{

    int res = stack[count - 1];
    stack[res] = 0;
    count --;
    return res;

}

int main()
{

    push(4);
    push(3);
    push(2);
    push(1);

    pop();

    for (int i = 0; i < 4; i ++)
    {
        if(stack[i] != 0)
            printf("%d\n", stack[i]);
    }


    return 0;
}