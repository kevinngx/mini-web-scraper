#include "list.h"
#include "string.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
    string data;
    struct Node *next;
} Node;

typedef struct List_Repr
{
    struct Node *head;
    size_t size;
} List_Repr;

list list_create(void)
{
    // Allocate memory for the list
    list l = malloc(sizeof(List_Repr));
    if (l == NULL) {
        // If there's an error, return NULL
        return NULL;
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
        return;

    Node *curr = l->head;
    Node *next = NULL;

    while (l->size != 0)
    {
        next = curr->next;
        if (curr->data != NULL) {
            free(curr->data);
        }
        free(curr);
        l->size--;
        curr = next;
    }
    l->head = NULL;
    l->size = 0;
}

bool list_is_empty(list l)
{
    if (l->size == 0)
        return true;
    else
        return false;
}

size_t list_length(list l)
{
    return l->size;
}

/****************************** STACK INTERFACE *******************************/
void list_push(list l, string s)
{
    if (l == NULL || s == NULL)
        return;
    Node *new = malloc(sizeof(Node));
    if (new == NULL) return;

    // Dynamically allocate memory for data and then copy data from string
    string input_data = malloc(strlen(s) + 1); // length of string + 1 for null terminator
    strcpy(input_data, s);
    new->data = input_data;
    new->next = l->head;

    l->head = new;
    l->size++;
}

string list_pop(list l)
{
    if (l == NULL || l->head == NULL)
        return NULL;
    Node *temp_head = l->head;
   
    // string data = l->head->data;
    string data = malloc(strlen(l->head->data) + 1);
    if (data == NULL) return NULL;
    strcpy(data, l->head->data);

    if (l->head->next != NULL)
    {
        l->head = l->head->next;
    }
    else
    {
        l->head = NULL;
    }
    l->size--;
    free(temp_head->data);
    free(temp_head);
    return data;
}

/****************************** QUEUE INTERFACE *******************************/
void list_enqueue(list l, string s)
{
    if (l == NULL || s == NULL)
        return;
    Node *new = malloc(sizeof(Node));
    if(new == NULL) return;

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
        return NULL;

    Node *curr = l->head;


    if (curr->next == NULL)
    {
        // If there is only one element
        string data = malloc(strlen(curr->data) + 1);
        if (data == NULL) return NULL;
        strcpy(data, curr->data);
        // string data = curr->data;
        l->size = 0;
        // free(curr->data);
        free(curr);
        l->head = NULL;
        return data;
    }
    else
    {
        // Stop at the second last element
        while (curr->next->next != NULL)
        {
            curr = curr->next;
        }
        string data = malloc(strlen(curr->next->data) + 1);
        if (data == NULL) return NULL;
        strcpy(data, curr->next->data);
        // string data = curr->next->data;
        // free(curr->next->data);
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
        return;

    Node *curr = l->head;

    // first node
    if (strcmp(curr->data, s) == 0)
    {
        l->head = curr->next;
        if (curr->data != NULL) {
            free(curr->data);
        }
        free(curr);
        l->size--;
        return;
    }

    // middle nodes
    while (curr->next != NULL)
    {
        if (strcmp(curr->next->data, s) == 0)
        {
            Node *temp = curr->next;
            curr->next = curr->next->next;
            if (temp->data != NULL) {
                free(temp->data);
            }
            free(temp);
            l->size--;
            return;
        }
        curr = curr->next;
    }

    // last node
    if (strcmp(curr->data, s) == 0)
    {
        if (curr->data != NULL) {
            free(curr->data);
        }
        free(curr);
        l->size--;
    }
    return;
}

bool list_contains(list l, string s)
{
    if (l == NULL || s == NULL)
        return false;
    Node *curr = NULL;
    curr = l->head;

    // List is empty
    if (curr == NULL)
        return false;

    // All elements
    while (curr->next != NULL)
    {
        if (strcmp(curr->data, s) == 0)
            return true;
        curr = curr->next;
    }

    // Final element
    if (strcmp(curr->data, s) == 0)
    {
        return true;
    }

    return false;
}
