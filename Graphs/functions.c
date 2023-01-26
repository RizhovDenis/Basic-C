static int *generator_categories(int num_categories){
    int category[num_categories];
    int *ptr = category;
    for (int i = 0; i < num_categories; i++){
        ptr[i] = i;
    }   
    return ptr;
}

static Node *create_node(int vertex){
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->vertex = vertex;
    new_node->next = NULL;
    return new_node;
}

static float recursion_search(Graph *graph, Node *temp, Node *new_temp, int src, int dest, int coef, float best_weight){
    while(temp){
        if (dest == temp->vertex){
            return best_weight;
        }
        temp = temp->next;
    }
    
    best_weight = best_weight / coef;
    new_temp = graph->head[new_temp->vertex];   
    return recursion_search(graph, new_temp, new_temp, src, dest, coef, best_weight);
}

Graph *create_graph(int capacity, int edges, int max_weight){
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->number_edges = edges;
    graph->capacity = capacity;
    graph->max_weight = max_weight;
    graph->head = malloc(capacity * sizeof(Node*));
    for (int i = 0; i < capacity; i++){
        graph->head[i] = NULL;
    }
    return graph;
}

Graph *create_edge(Graph *graph, int src, int dest){
    Node *new_node = create_node(dest); // add edge from src to dest
    new_node->next = graph->head[src];
    graph->head[src] = new_node;
    
    new_node = create_node(src);
    new_node->next = graph->head[dest];
    graph->head[dest] = new_node;
    
    return graph;
}

Graph *generator_edges(Graph *graph){
    int *ptr = generator_categories(graph->capacity);
    for (int j = 0; j < graph->number_edges; j++){

        int weight = rand() % graph->max_weight + 1;
        int new_source = rand() % graph->capacity;
        int new_dest = rand() % graph->capacity;

        if (new_source != new_dest){
            create_edge(graph, new_source, new_dest);
        }
        else{
            j--;
        }
    }   
    return graph;
}

void graph_relative(Graph *graph, int src, int dest, int coef, float best_weight){
    Node *temp = graph->head[src];
    Node *new_temp = graph->head[src];
    best_weight = recursion_search(graph, temp, new_temp, src, dest, coef, best_weight);
    printf("\n %d->%d (%.2f) \n", src, dest, best_weight);
}

void show_graph(Graph *graph){
    for (int i = 0; i < graph->capacity; i++){
        if (graph->head[i]==NULL){
            continue;
        }

        Node *temp = graph->head[i];
        printf("\n Vertex %d:\n", i);
        while(temp){
            printf(" %d->%d ", i, temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}