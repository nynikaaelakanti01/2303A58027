#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20 


struct Edge {
    int src, dest, weight;
};


struct Subset {
    int parent;
    int rank;
};


int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}


void Union(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}


int compareEdges(const void* a, const void* b) {
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return a1->weight - b1->weight;
}


void kruskalMST(struct Edge edges[], int V, int E, char names[][MAX]) {
    struct Edge result[V]; 
    int e = 0; 
    int i = 0; 
    int totalCost = 0;

    
    qsort(edges, E, sizeof(edges[0]), compareEdges);

    
    struct Subset* subsets = (struct Subset*) malloc(V * sizeof(struct Subset));

  
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

   
    while (e < V - 1 && i < E) {
        struct Edge next_edge = edges[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

   
        if (x != y) {
            result[e++] = next_edge;
            totalCost += next_edge.weight;
            Union(subsets, x, y);
        }
    }


    printf("Selected Edges in the MST:\n");
    for (i = 0; i < e; ++i)
        printf("%s - %s : %d\n", names[result[i].src], names[result[i].dest], result[i].weight);

    printf("\nTotal Minimum Cost = %d\n", totalCost);

    free(subsets);
}


int getIndex(char names[][MAX], int V, char name[]) {
    for (int i = 0; i < V; i++) {
        if (strcmp(names[i], name) == 0)
            return i;
    }
    return -1;
}

int main() {
    int V, E;
    printf("Enter number of vertices: ");
    scanf("%d", &V);
    printf("Enter number of edges: ");
    scanf("%d", &E);

    char names[V][MAX];
    printf("Enter vertex names:\n");
    for (int i = 0; i < V; i++) {
        scanf("%s", names[i]);
    }

    struct Edge edges[E];
    printf("Enter edges (u v cost):\n");
    for (int i = 0; i < E; i++) {
        char u[MAX], v[MAX];
        int cost;
        scanf("%s %s %d", u, v, &cost);
        edges[i].src = getIndex(names, V, u);
        edges[i].dest = getIndex(names, V, v);
        edges[i].weight = cost;
    }

    kruskalMST(edges, V, E, names);

    return 0;
}