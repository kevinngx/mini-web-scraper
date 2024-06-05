#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "graph.h"

/// ------------------ for testing only, remove these ------------------

// Our vertexes here are strings, each with a list of edges
typedef struct Vertex {
    string label;
    list edges;
} Vertex;

// Each edge has a destination and weight (do not need source)
typedef struct Edge {
    string destination;
    size_t weight;
} Edge;

// Adjacency List Representation
typedef struct Graph_Repr
{
    list vertices;
    // int nV; // Lectures had these but not sure if we need
    // int nE; // For counts, we can just loop through the adjacency lists
} Graph_Repr;

void add_or_increment_edge(graph g, string vertex1, string vertex2)
{
    printf("\n\n-----------------add_or_increment: vertex1 = %s, vertex2 = %s -----------------\n", vertex1, vertex2);
    list ignore_list = list_create();
    printf("Graph before: \n");
    graph_show(g, NULL, ignore_list);

    printf("Checking if graph has vertex\n"); // REMOVE THIS LINE
    graph_show(g, NULL, NULL); //REMOVE THIS LINE
    if (!graph_has_vertex(g, vertex1)) graph_add_vertex(g, vertex1);
    if (!graph_has_vertex(g, vertex2)) graph_add_vertex(g, vertex2);

    printf("Checking if graph has edge\n"); // REMOVE THIS LINE
    if (!graph_has_edge(g, vertex1, vertex2)) graph_add_edge(g, vertex1, vertex2,   1);
    else graph_set_edge(g, vertex1, vertex2,  graph_get_edge(g, vertex1, vertex2) + 1);

    printf("add_or_increment_success!\n");
    printf("Graph after: \n");
    graph_show(g, NULL, ignore_list);
}

/// ------------------ for testing only, remove these ------------------

int main()
{
    printf("Hello World, Testing Graphs\n");

    // Test Create
    graph urlGraph = graph_create();
    // string base_url = "http://www.cse.unsw.edu.au/~cs9024/micro-web/X.html";
    // string link = "http://www.cse.unsw.edu.au/~cs9024/micro-web/index.html";
    // add_or_increment_edge(urlGraph, base_url, link);

    // base_url = "http://www.cse.unsw.edu.au/~cs9024/micro-web/index.html";
    // link = "http://www.cse.unsw.edu.au/~cs9024/micro-web/Y.html";
    // add_or_increment_edge(urlGraph, base_url, link);
    

    // Test adding vertices
    printf("\n----2. Adding vertices----\n");
    graph_has_vertex(urlGraph, "Hello World");
    graph_add_vertex(urlGraph, "URL: INDEX");
    graph_add_vertex(urlGraph, "URL: HOME");
    graph_add_vertex(urlGraph, "URL: PAGE-X");
    graph_add_vertex(urlGraph, "URL: PAGE-Y");
    graph_add_vertex(urlGraph, "URL: PAGE-Z");
    graph_add_vertex(urlGraph, "URL: HOME");

    // Test adding edges
    printf("\n----2. Adding Edges----\n");
    graph_add_edge(urlGraph, "URL: HOME", "URL: PAGE-X", 10);
    graph_add_edge(urlGraph, "URL: HOME", "URL: PAGE-Y", 15);
    // graph_add_edge(urlGraph, "URL: HOME", "URL: PAGE-Z", 20);
    graph_add_edge(urlGraph, "URL: PAGE-A", "URL: PAGE-B", 15);

    // Checking edges exist
    printf("\n----3. Checking Edges Exist ----\n");
    printf("Graph HOME->X : %d\n", graph_has_edge(urlGraph, "URL: HOME", "URL: PAGE-X"));
    printf("Graph HOME->Y : %d\n", graph_has_edge(urlGraph, "URL: HOME", "URL: PAGE-Y"));
    printf("Graph HOME->Z : %d\n", graph_has_edge(urlGraph, "URL: HOME", "URL: PAGE-Z"));
    printf("Graph A->B : %d\n", graph_has_edge(urlGraph, "URL: PAGE-A", "URL: PAGE-B"));

    // // Test showing
    // printf("\n----4. Show Graph ----\n");
    list ignore_list = list_create();
    // graph_show(urlGraph, NULL, ignore_list);
    // // TODO: Need to implement an ignore list

    // // Test Updating Vertices
    // printf("\n----5. Updating Vertices ----\n");
    // graph_set_edge(urlGraph, "URL: HOME", "URL: PAGE-X", 20);
    // graph_set_edge(urlGraph, "URL: PAGE-A", "URL: PAGE-B", 30);
    // graph_show(urlGraph, NULL, ignore_list);

    // // Test getting edges
    // printf("\n----6. Getting edges ----\n");
    // printf("Edge for A->B is %zu\n", graph_get_edge(urlGraph, "URL: PAGE-A", "URL: PAGE-B"));
    // graph_set_edge(urlGraph, "URL: PAGE-A", "URL: PAGE-B", 50);
    // printf("Edge for A->B is %zu\n", graph_get_edge(urlGraph, "URL: PAGE-A", "URL: PAGE-B"));


    // // Test graph_edge_count()
    // printf("\n----7. Counting edges ----\n");
    // printf("Vertex A has %zu edges\n", graph_edges_count(urlGraph, "URL: PAGE-A"));
    // printf("Vertex HOME has %zu edges\n", graph_edges_count(urlGraph, "URL: HOME"));


    // // Test Ignore List and File Write Out
    // printf("\n----8. Ignore List for Showing Graph----\n");
    // graph_show(urlGraph, NULL, ignore_list);
    // list_add(ignore_list, "URL: PAGE-A");
    // list_add(ignore_list, "URL: PAGE-X");
    // graph_show(urlGraph, NULL, ignore_list);

    // // Test writing to file
    printf("\n----9. Writing to file----\n");
    FILE *output_file = fopen("my_output.txt", "w");
    graph_show(urlGraph, output_file, ignore_list);

    // Test destroying
    graph_destroy(urlGraph);
    printf("Done.\n");

    return 0;
}
