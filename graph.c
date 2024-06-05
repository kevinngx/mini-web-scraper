#include "graph.h"
#include "pagerank.h"
#include "list.h"
#include "math.h"
#include "dijkstra.h"

#include "string.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_NODES 1000
#define VERY_HIGH_VALUE 999999

// Our vertexes here are strings, each with a list of edges
typedef struct Vertex
{
    string label;
    struct Edge *edges;
    struct Vertex *next;
    double old_rank;
    double page_rank;
} Vertex;

// Each edge has a destination and weight (do not need source)
typedef struct Edge
{
    string destination;
    size_t weight;
    struct Edge *next;
} Edge;

// Adjacency List Representation
typedef struct Graph_Repr
{
    Vertex *vertices_head;
    // int nV; // Lectures had these but not sure if we need
    // int nE; // For counts, we can just loop through the adjacency lists instead of maintaining an internal count.
} Graph_Repr;

typedef struct
{
    char *label;
    double page_rank;
} NodeInfo;

/***************************** HELPER FUNCTIONS ******************************/

void edge_list_destroy(Edge *edges)
{
    // If there are no edges, then we do nothing
    if (edges == NULL)
    {
        return;
    }

    Edge *curr = edges;
    Edge *prev = NULL;

    // Loop through the list of edges and remove each node
    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
        free(prev->destination);
        free(prev);
    }

    // Remove the final node
    free(curr);
}

bool vertex_has_edge(Edge *edge_list, string destination)
{
    // printf("Checking if this vertex contains [%s]\n", destination);

    if (edge_list == NULL || edge_list->destination == NULL)
    {
        return false;
    }

    // Edge *curr_edge = NULL;
    // curr_edge = edge_list;
    Edge *curr_edge = edge_list;

    while (curr_edge != NULL)
    {
        if (strcmp(curr_edge->destination, destination) == 0)
        {
            return true;
        }
        curr_edge = curr_edge->next;
    }

    return false;
}

/***************************** GRAPH INTERFACE ******************************/

graph graph_create(void)
{
    // Allocate memory for the list
    graph g = malloc(sizeof(Graph_Repr));
    assert(g != NULL);

    g->vertices_head = NULL;
    return g;
}

void graph_destroy(graph g)
{
    if (g == NULL || g->vertices_head == NULL)
    {
        return;
    }

    Vertex *curr = g->vertices_head;
    Vertex *prev = NULL;

    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
        edge_list_destroy(curr->edges);
        free(prev);
    }
    free(curr);
}

void show_vertex_edges(Edge *edge_list, FILE *f, string label, list ignore_list)
{
    if (edge_list == NULL)
    {
        return;
    }

    Edge *curr_edge = edge_list;

    while (curr_edge != NULL)
    {
        // for all edges linked to the vertex
        if (list_contains(ignore_list, curr_edge->destination) == false)
        {
            if (f != NULL)
            {
                fprintf(f, "%s %s %zu\n", label, curr_edge->destination, curr_edge->weight);
            }
            else
            {
                printf("%s %s %zu\n", label, curr_edge->destination, curr_edge->weight);
            }
        }
        curr_edge = curr_edge->next;
    }
}

void graph_show(graph g, FILE *f, list ignore_list)
{
    if (g == NULL || ignore_list == NULL)
    {
        return;
    }

    // Print out vertices
    Vertex *curr_vertex = g->vertices_head;

    while (curr_vertex != NULL)
    {
        // For all vertexes
        if (list_contains(ignore_list, curr_vertex->label) == false)
        {
            if (f != NULL)
            {
                fprintf(f, "%s\n", curr_vertex->label);
            }
            else
            {
                printf("%s\n", curr_vertex->label);
            }
        }
        curr_vertex = curr_vertex->next;
    }

    // Print out edges
    curr_vertex = g->vertices_head;
    while (curr_vertex != NULL)
    {
        if (list_contains(ignore_list, curr_vertex->label) == false)
        {
            show_vertex_edges(curr_vertex->edges, f, curr_vertex->label, ignore_list);
        }

        curr_vertex = curr_vertex->next;
    }
}

void graph_add_vertex(graph g, string s)
{
    // Return if we are given NULLS
    if (g == NULL || s == NULL)
        return;

    // Only add new vertex if it is not currently in the list
    if (!graph_has_vertex(g, s))
    {
        // printf("Adding Vertex : %s\n", s);
        Vertex *newVertex = malloc(sizeof(Vertex));
        newVertex->edges = malloc(sizeof(Edge));
        newVertex->edges = NULL;
        string input_data = malloc(strlen(s) + 1);
        strcpy(input_data, s);
        newVertex->label = input_data;
        newVertex->next = NULL;

        // Prepend to the list
        if (g->vertices_head == NULL)
        {
            // Add the new Vertex as the head of ther vertices list
            g->vertices_head = newVertex;
            return;
        }
        else
        {
            // Append to the end
            Vertex *curr = g->vertices_head;
            while (curr->next != NULL)
            {
                curr = curr->next;
            }
            curr->next = newVertex;
        }
    }
}

bool graph_has_vertex(graph g, string s)
{
    // If graph is empty, then return false
    if (g == NULL || g->vertices_head == NULL || s == NULL)
    {
        return false;
    }

    // Check all vartex in g->vertices if vertex-> label is equal to the string
    Vertex *curr = g->vertices_head;

    // Check all elements
    while (curr != NULL)
    {
        if (strcmp(curr->label, s) == 0)
            return true;
        curr = curr->next;
    }
    return false;
}

size_t graph_vertices_count(graph g)
{
    if (g == NULL || g->vertices_head == NULL)
    {
        return 0;
    }

    Vertex *curr = g->vertices_head;
    int count = 1;

    while (curr->next != NULL)
    {
        count++;
        curr = curr->next;
    }
    return count;
}

void graph_add_edge(graph g, string source, string destination, size_t weight)
{
    // If any inputs are NULL, do nothing
    if (g == NULL || g->vertices_head == NULL || source == NULL || destination == NULL)
        return;

    // printf("Attempting to add edge: [%s]--%zu-->[%s]\n", source, weight, destination);

    // If graph already has this edge, do nothing
    // printf("Checking if the graph already has edge\n");
    if (graph_has_edge(g, source, destination))
    {
        return;
    }

    if (!graph_has_vertex(g, source))
    {
        graph_add_vertex(g, source);
    }

    if (!graph_has_vertex(g, destination))
    {
        graph_add_vertex(g, destination);
    }

    // printf("Attempting to add edge: \n");
    Vertex *curr_vertex = g->vertices_head;
    Edge *newEdge = malloc(sizeof(Edge));
    string dest = malloc(strlen(destination) + 1);
    strcpy(dest, destination);
    newEdge->destination = dest;
    newEdge->weight = weight;
    newEdge->next = NULL;

    while (curr_vertex != NULL)
    {
        // We enqueue a new edge IF the graph does not yet have this edge
        if (strcmp(curr_vertex->label, source) == 0)
        {
            if (curr_vertex->edges == NULL)
            {
                curr_vertex->edges = newEdge;
            }
            else
            {
                Edge *curr_edge = curr_vertex->edges;
                while (curr_edge->next != NULL)
                {
                    curr_edge = curr_edge->next;
                }
                curr_edge->next = newEdge;
            }
            // printf("[%s]-%zu->[%s] Succesfully added!\n", source, weight, destination);
        }
        curr_vertex = curr_vertex->next;
    }
}

bool graph_has_edge(graph g, string source, string destination)
{
    // If any inputs are NULL, do nothing
    if (g == NULL || g->vertices_head == NULL || source == NULL || destination == NULL)
        false;

    // printf("Checking if graph already contains [%s]-->[%s]\n", source, destination);

    // If the source or destination are not vertices on the graph, return false
    if (!graph_has_vertex(g, source) || !graph_has_vertex(g, destination))
        return false;

    Vertex *curr_vertex = g->vertices_head;
    while (curr_vertex != NULL)
    {
        if (strcmp(curr_vertex->label, source) == 0)
        {
            // Check if source has edge connecting to the destination
            return vertex_has_edge(curr_vertex->edges, destination);
        }
        curr_vertex = curr_vertex->next;
    }
    return false;
}

void vertex_set_edge(Edge *edge_list, string destination, size_t weight)
{
    if (edge_list == NULL)
        return;
    Edge *curr_edge = edge_list;
    while (curr_edge != NULL)
    {
        if (strcmp(curr_edge->destination, destination) == 0)
        {
            curr_edge->weight = weight;
            return;
        }
        curr_edge = curr_edge->next;
    }
}

void graph_set_edge(graph g, string source, string destination, size_t weight)
{
    if (g == NULL || g->vertices_head == NULL || source == NULL || destination == NULL)
        return;

    // If the source or destination are not vertices on the graph, return false
    if (!graph_has_vertex(g, source) || !graph_has_vertex(g, destination))
        return;

    // Loop until we find the source vertex and then set the edge
    Vertex *curr_vertex = g->vertices_head;
    while (curr_vertex != NULL)
    {
        if (strcmp(curr_vertex->label, source) == 0)
        {
            // Check if source has edge connecting to the destination
            vertex_set_edge(curr_vertex->edges, destination, weight);
            return;
        }
        curr_vertex = curr_vertex->next;
    }
}

size_t vertex_get_edge(Edge *edge_list, string destination)
{
    if (edge_list == NULL)
        return 0;

    Edge *curr_edge = edge_list;
    while (curr_edge != NULL)
    {
        if (strcmp(curr_edge->destination, destination) == 0)
        {
            return curr_edge->weight;
        }
        curr_edge = curr_edge->next;
    }

    return 0;
}

size_t graph_get_edge(graph g, string source, string destination)
{
    if (g == NULL || g->vertices_head == NULL || source == NULL || destination == NULL)
        return 0;

    // If the source or destination are not vertices on the graph, return 0
    if (!graph_has_vertex(g, source) || !graph_has_vertex(g, destination))
        return 0;

    // Loop until we find the source vertex and then set the edge
    Vertex *curr_vertex = g->vertices_head;
    while (curr_vertex != NULL)
    {
        if (strcmp(curr_vertex->label, source) == 0)
        {
            // Check if source has edge connecting to the destination
            return vertex_get_edge(curr_vertex->edges, destination);
        }
        curr_vertex = curr_vertex->next;
    }

    return 0;
}

size_t vertex_edge_count(Edge *edge_list)
{
    if (edge_list == NULL)
        return 0;

    Edge *curr_edge = edge_list;
    size_t count = 1;

    while (curr_edge->next != NULL)
    {
        count++;
        curr_edge = curr_edge->next;
    }

    return count;
}

size_t graph_edges_count(graph g, string source)
{
    if (g == NULL || g->vertices_head == NULL || source == NULL)
        return 0;

    // If the source vertex is not on the graph, return 0
    if (!graph_has_vertex(g, source))
        return 0;

    Vertex *curr_vertex = g->vertices_head;
    while (curr_vertex != NULL)
    {
        if (strcmp(curr_vertex->label, source) == 0)
        {
            // Check if source has edge connecting to the destination
            return vertex_edge_count(curr_vertex->edges);
        }
        curr_vertex = curr_vertex->next;
    }

    return 0;
}

/***************************** PAGERANK INTERFACE ******************************/

size_t vertex_edge_count_with_ignore(Edge *edge_list, list ignore_list)
{
    if (edge_list == NULL || ignore_list == NULL)
        return 0;

    Edge *curr_edge = edge_list;
    size_t count = 0;

    while (curr_edge != NULL)
    {
        if (list_contains(ignore_list, curr_edge->destination) == false)
        {
            count++;
        }
        curr_edge = curr_edge->next;
    }
    return count;
}

size_t graph_vertices_count_with_ignore(graph g, list ignore_list)
{
    if (g == NULL || g->vertices_head == NULL)
        return 0;

    Vertex *curr = g->vertices_head;
    int count = 0;

    while (curr != NULL)
    {
        if (list_contains(ignore_list, curr->label) == false)
        {
            count++;
        }
        curr = curr->next;
    }
    return count;
}

void page_rank_vertices(graph g, Vertex *V, double damping_factor, list ignore_list)
{
    if (g == NULL || V == NULL || ignore_list == NULL)
        return;
    // for all I in vertices of G that have an edge from I to V
    //     pagerank of V = pagerank of V + ((damping_factor * oldrank of I) / number of outbound edges from I)
    // end for
    Vertex *I = NULL;
    I = g->vertices_head;
    if (I == NULL)
        return;

    double n;
    while (I != NULL)
    {
        if (list_contains(ignore_list, I->label) == false && graph_has_edge(g, I->label, V->label))
        {
            n = vertex_edge_count_with_ignore(I->edges, ignore_list);
            V->page_rank = V->page_rank + ((damping_factor * I->old_rank) / n);
        }
        I = I->next;
    }
}

void graph_pagerank(graph g, double damping_factor, double epsilon, list ignore_list)
{
    // procedure graph_pagerank(G, damping_factor, epsilon)
    if (g == NULL || g->vertices_head == NULL)
        return;

    // N = number of vertices in G
    // double n = graph_vertices_count(g) - list_length(ignore_list);
    double n = graph_vertices_count_with_ignore(g, ignore_list);

    Vertex *V = NULL;
    V = g->vertices_head;
    if (V == NULL)
        return;

    while (V != NULL)
    {
        V->old_rank = 0.0;
        V->page_rank = 1.0 / n;
        V = V->next;
    }

    bool converged;
    do
    {
        converged = true;

        V = g->vertices_head;
        if (V == NULL)
            return;
        while (V != NULL)
        {
            V->old_rank = V->page_rank;
            V = V->next;
        }

        // sink_rank = 0
        double sink_rank = 0;

        // Adjust the 'sink' value
        V = g->vertices_head;
        if (V == NULL)
            return;
        while (V != NULL)
        {
            if (list_contains(ignore_list, V->label) == false && vertex_edge_count_with_ignore(V->edges, ignore_list) == 0)
            {
                sink_rank = sink_rank + (damping_factor * (V->old_rank / n));
            }
            V = V->next;
        }

        // Update PageRank values
        V = g->vertices_head;
        if (V == NULL)
            return;
        while (V != NULL)
        {
            if (list_contains(ignore_list, V->label) == false)
            {
                V->page_rank = sink_rank + ((1 - damping_factor) / n);
                page_rank_vertices(g, V, damping_factor, ignore_list);
                // printf("Page Rank [%s] = %.3f\n", V->label, V->page_rank);
            }

            // Check for convergence
            if (list_contains(ignore_list, V->label) == false && fabs(V->page_rank - V->old_rank) > epsilon)
            {
                converged = false;
            }
            V = V->next;
        }
    } while (!converged);
}

// Comparison function for qsort
int compare_page_rank(const void *a, const void *b)
{
    const NodeInfo *nodeA = (const NodeInfo *)a;
    const NodeInfo *nodeB = (const NodeInfo *)b;

    if (nodeA->page_rank > nodeB->page_rank)
        return -1; // Descending order
    if (nodeA->page_rank < nodeB->page_rank)
        return 1;
    return 0;
}

void graph_show_pagerank(graph g, FILE *f, list ignore_list)
{
    if (g == NULL || ignore_list == NULL)
        return;

    // Calculate array size
    int num_nodes = 0;
    Vertex *curr_vertex = g->vertices_head;
    while (curr_vertex != NULL)
    {
        if (list_contains(ignore_list, curr_vertex->label) == false)
        {
            num_nodes++;
        }
        curr_vertex = curr_vertex->next;
    }

    // Create array to store node data
    NodeInfo *node_info = malloc(num_nodes * sizeof(NodeInfo));
    if (node_info == NULL)
    {
        fprintf(stderr, "Error allocating memory\n");
        return;
    }

    // Populate array
    int i = 0;
    curr_vertex = g->vertices_head;
    while (curr_vertex != NULL)
    {
        if (list_contains(ignore_list, curr_vertex->label) == false)
        {
            node_info[i].label = curr_vertex->label;
            node_info[i].page_rank = curr_vertex->page_rank;
            i++;
        }
        curr_vertex = curr_vertex->next;
    }

    // Sort array by page_rank (descending) using qsort
    qsort(node_info, num_nodes, sizeof(NodeInfo), compare_page_rank);

    // Print sorted vertices
    for (i = 0; i < num_nodes; i++)
    {
        if (f != NULL)
        {
            fprintf(f, "%s: %.3f\n", node_info[i].label, node_info[i].page_rank);
        }
        else
        {
            printf("%s: %.3f\n", node_info[i].label, node_info[i].page_rank);
        }
    }

    free(node_info);
}

/***************************** DIJKSTRA INTERFACE ******************************/



void graph_shortest_path(graph g, string source, list ignore_list)
{
    printf("graph_shortest_path()\n");
}

void graph_show_path(graph g, FILE *out_file, string destination, list ignore_list)
{
    printf("graph_show_path()\n");
}