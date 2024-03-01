#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Node{

    int data;
    struct Node* next;
    struct Node* prev;

}Node;

struct Node* head = NULL;
struct Node* tail = NULL;


// adds node to end of Linked list
Node* addNode(int data)
{
    // if list is empty then add node to list and make it the head
    if (head == NULL)
    {
        // allocate memory for new node
         Node* newNode = malloc(sizeof(Node));

         // assign new node to be the head
         head = newNode;

         // initialize heads variables
         head->data = data;
         head->prev = NULL;
         head->next = NULL;
    }
}

// inserts node in specific location
Node* insertNode(int data)
{
    
}

// deletes node from linked list
Node* deleteNode(int data)
{
    
}

// checks to see if a node is in the list
bool nodeInList(int data)
{

}

// reverses order of list
Node* reverse()
{

}

// makes list a circular linked list
Node* makeCircular()
{

}

// prints list
void printLL(Node* head)
{

}





int main()
{



    return 0; 
}