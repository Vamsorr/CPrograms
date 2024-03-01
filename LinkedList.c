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


// adds node to end of Linked list and returns the head of the LL
Node* addNode(int data)
{
    // allocating memory for the new node
    Node* newNode = malloc(sizeof(Node));

    // initializing new nodes data
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = tail;
    
    // if head is equal to NULL 
    if (head == NULL)
    {
        head = newNode;
        tail = newNode;
    }

    else
    {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }

    
 return head;
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
    // create current node to iterate through list
    Node* curr = head;

    // iterate through linked list
    while (curr != NULL)
    {
        printf("%d ->  ", curr->data);
        curr = curr->next;
    }

    printf("NULL\n");
}





int main()
{
    addNode(1);
    addNode(2);
    addNode(3);
    addNode(6);

    printLL(head);


    return 0; 
}