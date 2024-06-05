#include "list.h"
#include "string.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
    string data;
    struct Node *next;
} Node; // A single node in the linked list

typedef struct List_Repr
{
    struct Node *head;
    size_t size;
} List_Repr; // Internal representation of the list

list list_create(void)
{
    // Allocate memory for the list
    list l = malloc(sizeof(List_Repr));
    if (l == NULL)
    {
        return NULL; // return NULL if we aren't able to allocate memory
    }

    // Assign default values
    l->head = NULL;
    l->size = 0;

    return l;
}

void list_destroy(list l)
{
    // If the list is empty, do nothing
    if (l == NULL)
    {
        return;
    }

    Node *curr = l->head;
    Node *next = NULL;

    while (l->size != 0) // Iterate until the list is empty
    {
        next = curr->next;
        if (curr->data != NULL)
        {
            free(curr->data); // Only free data if we have allocated memory for data and assigned it to the ndoe
        }
        free(curr); // Free the node
        l->size--;
        curr = next;
    }
    free(l); // Free the final list
}

bool list_is_empty(list l)
{
    if (l->size == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

size_t list_length(list l)
{
    return l->size;
}

/****************************** STACK INTERFACE *******************************/

void list_push(list l, string s)
{
    if (l == NULL || s == NULL)
    {
        return;
    }

    // Dynamically allocate memory for data and then copy data from string
    Node *new = malloc(sizeof(Node));
    string input_data = malloc(strlen(s) + 1); // length of string + 1 for null terminator

    if (new == NULL || input_data == NULL)
    {
        return;
    }
    strcpy(input_data, s);

    // We add the new node to the front of the linked list
    new->data = input_data;
    new->next = l->head;
    l->head = new;
    l->size++;
}

string list_pop(list l)
{
    if (l == NULL || l->head == NULL)
    {
        return NULL;
    }

    Node *temp_head = l->head;
    string data = l->head->data;
    l->head = l->head->next;
    l->size--;
    free(temp_head);
    return data;
}

/****************************** QUEUE INTERFACE *******************************/

void list_enqueue(list l, string s)
{
    if (l == NULL || s == NULL)
    {
        return;
    }

    Node *new = malloc(sizeof(Node));
    if (new == NULL)
    {
        return;
    }

    string input_data = malloc(strlen(s) + 1); // length of string + 1 for null terminator
    strcpy(input_data, s);
    new->data = input_data;
    new->next = l->head;

    l->head = new;
    l->size++;
}

string list_dequeue(list l)
{
    if (l == NULL || l->head == NULL)
    {
        return NULL;
    }

    Node *curr = l->head;

    if (curr->next == NULL)
    {
        // Special case: list has only one element
        string data = curr->data;
        l->size = 0;
        free(curr);
        l->head = NULL;
        return data;
    }
    else
    {
        // For all other cases, stop at the second last element
        while (curr->next->next != NULL)
        {
            curr = curr->next;
        }
        string data = curr->next->data;
        free(curr->next);
        curr->next = NULL;
        l->size--;
        return data;
    }

    return NULL;
}

/******************************* SET INTERFACE ********************************/

void list_add(list l, string s)
{
    if (!list_contains(l, s))
    {
        list_enqueue(l, s);
    }
}

void list_remove(list l, string s)
{
    if (l == NULL || l->head == NULL || s == NULL)
    {
        return;
    }

    Node *curr = l->head;

    // Handle removing the head node
    if (strcmp(curr->data, s) == 0)
    {
        l->head = curr->next;
        if (curr->data != NULL)
        {
            free(curr->data);
        }
        free(curr);
        l->size--;
        return;
    }

    // Handle removing all other nodes
    while (curr->next != NULL)
    {
        curr = curr->next;
        if (strcmp(curr->next->data, s) == 0)
        {
            Node *temp = curr->next;
            curr->next = curr->next->next;
            if (temp->data != NULL)
            {
                free(temp->data);
            }
            free(temp);
            l->size--;
            return;
        }
    }
    return;
}

bool list_contains(list l, string s)
{
    if (l == NULL || s == NULL)
    {
        return false;
    }

    Node *curr = l->head;

    // Scan list for element
    while (curr != NULL)
    {
        if (strcmp(curr->data, s) == 0)
            return true;
        curr = curr->next;
    }

    return false;
}
