#define EDGES 5 // maximum number of edges in the graph
#define CAPACITY 10 // number unique categories 
#define MAX_WEIGHT 10.0
#define COEFFICIENT 2.0 // for relative searching

typedef struct Node{
    int vertex;
    struct Node *next;
}Node;

typedef struct Graph{
    int max_weight;
    int number_edges;
    int capacity;
    struct Node **head;
}Graph;

Graph *create_graph(int capacity, int edges, int max_weight);
Graph *create_edge(Graph *graph, int src, int dest);
Graph *generator_edges(Graph *graph);
void graph_relative(Graph *graph, int src, int dest, int coef, float best_weight);
void show_graph(Graph *graph);