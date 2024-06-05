#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/// ------------------ for testing only, remove these ------------------

typedef struct Node
{
    string data;
    struct Node *next;
} Node;

typedef struct List_Repr
{
    struct Node *head;
    struct Node *tail;
    size_t size;
} List_Repr;

void list_show(list l)
{
    if (l == NULL || l->head == NULL)
        return;
    Node *curr = l->head;
    while (curr->next != NULL)
    {
        printf("[%s]-->", curr->data);
        curr = curr->next;
    }
    printf("[%s]\n", curr->data);
}

/// ------------------ for testing only, remove these ------------------

int main()
{

    string line;
    string data[4] = {"http://www.cse.unsw.edu.au/~cs9024/micro-web/index.html",
                      "http://www.cse.unsw.edu.au/~cs9024/micro-web/X.html",
                      "http://www.cse.unsw.edu.au/~cs9024/micro-web/Y.html",
                      "http://www.cse.unsw.edu.au/~cs9024/micro-web/Z.html"};

    // string data[4] = {"1", "2", "3", "4"};

    // Test Case #1 Stack
    printf("\n--------------------- Test Case #1 Stack ---------------------\n");
    list stack = list_create();
    printf("Stack height: %zu\n", list_length(stack));

    // Pushing data to the stack
    for (int i = 0; i < 4; i++)
    {
        line = data[i];
        printf("Push: %s\n", line);
        list_push(stack, line);
    }

    printf("Stack height: %zu\n", list_length(stack));

    // Popping data from the stack
    while (!list_is_empty(stack))
    {
        line = list_pop(stack);
        printf("Pop: %s\n", line);
        free(line);
    }

    // Test Case #2 Queue
    printf("\n--------------------- Test Case #2 Queue ---------------------\n");
    list queue = list_create();

    // Pushing data to the stack
    printf("Queue length: %zu\n", list_length(queue));
    for (int i = 0; i < 4; i++)
    {
        line = data[i];
        printf("Push: %s\n", line);
        list_enqueue(queue, line);
    }

    printf("Queue length: %zu\n", list_length(queue));

    // dequeuing data from the queue
    while (!list_is_empty(queue))
    {
        line = list_dequeue(queue);
        printf("Dequeue: %s\n", line);
    }

    // Test Case #2 Queue
    printf("\n--------------------- Test Case #3 Set ---------------------\n");
    list set = list_create();

    printf("Set size: %zu\n", list_length(set));

    // line = "http://www.cse.unsw.edu.au/~cs9024/micro-web/index.html";
    // printf("Add: %s\n", line);
    // list_add(set, line);

    // line = "http://www.cse.unsw.edu.au/~cs9024/micro-web/index.html";
    // printf("Remove: %s\n", line);
    // list_remove(set, line);

    // line = "http://www.cse.unsw.edu.au/~cs9024/micro-web/X.html";
    // printf("Add: %s\n", line);
    // list_add(set, line);

    // line = "http://www.cse.unsw.edu.au/~cs9024/micro-web/Y.html";
    // printf("Add: %s\n", line);
    // list_add(set, line);

    // line = "http://www.cse.unsw.edu.au/~cs9024/micro-web/Y.html";
    // printf("Remove: %s\n", line);
    // list_remove(set, line);

    // line = "http://www.cse.unsw.edu.au/~cs9024/micro-web/Z.html";
    // printf("Add: %s\n", line);
    // list_add(set, line);

    // Pushing data to the stack
    line = "index";
    printf("Add: %s\n", line);
    list_add(set, line);

    line = "index";
    printf("Remove: %s\n", line);
    list_remove(set, line);

    line = "X";
    printf("Add: %s\n", line);
    list_add(set, line);

    line = "Y";
    printf("Add: %s\n", line);
    list_add(set, line);

    line = "Y";
    printf("Remove: %s\n", line);
    list_remove(set, line);

    line = "Z";
    printf("Add: %s\n", line);
    list_add(set, line);

    printf("Set size: %zu\n", list_length(set));
    
    printf("Final List: ");
    list_show(set);

    // Test Case #4 List Contains
    printf("\n--------------------- Test Case #4 List Contains ---------------------\n");
    stack = list_create();
    printf("Stack height: %zu\n", list_length(stack));

    // Pushing data to the stack
    for (int i = 0; i < 4; i++)
    {
        line = data[i];
        printf("Push: %s\n", line);
        list_push(stack, line);
    }

    string search = "http://www.cse.unsw.edu.au/~cs9024/ro-web/X.html";
    printf("Checking list contains: %s\n", search);
    bool contains = list_contains(stack, search);
    if (contains) {
        printf("Contains!\n");
    } else {
        printf("Does not contain\n");
    }
    
    return 0;
}
