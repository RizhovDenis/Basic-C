#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "functions.c"

int main(){
    Graph *graph = create_graph(CAPACITY, EDGES, MAX_WEIGHT);
    generator_edges(graph);
    show_graph(graph);
    graph_relative(graph, 5, 2, COEFFICIENT, graph->max_weight);
    return 0;
}