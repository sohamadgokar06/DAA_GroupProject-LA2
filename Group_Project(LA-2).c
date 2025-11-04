// Smart Logistics System 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 20

// Structure for an edge (Kruskal)
struct Edge {
    int src, dest, weight;
};

// Structure for subset (Union–Find)
struct Subset {
    int parent;
    int rank;
};

// GLOBAL VARIABLES 
char cities[MAX][50];
int graph[MAX][MAX];
int V, E;

//  UTILITY 
void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

//  UNION–FIND HELPERS 
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

int compareEdges(const void *a, const void *b) {
    struct Edge *a1 = (struct Edge *)a;
    struct Edge *b1 = (struct Edge *)b;
    return a1->weight - b1->weight;
}

//  KRUSKAL’S ALGORITHM 
void KruskalMST(struct Edge edges[], int V, int E) {
    struct Edge result[V];
    int e = 0, i = 0;
    qsort(edges, E, sizeof(edges[0]), compareEdges);

    struct Subset *subsets = (struct Subset *)malloc(V * sizeof(struct Subset));
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    printf("\n Optimal Road Network (Kruskal’s MST):\n");
    printf("-----------------------------------------\n");

    int minCost = 0;
    while (e < V - 1 && i < E) {
        struct Edge next_edge = edges[i++];
        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        if (x != y) {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }

    for (i = 0; i < e; ++i) {
        printf(" %s <-> %s : %d km\n",
               cities[result[i].src],
               cities[result[i].dest],
               result[i].weight);
        minCost += result[i].weight;
    }
    printf("-----------------------------------------\n");
    printf(" Minimum total road distance: %d km\n", minCost);
    free(subsets);
}

//  DIJKSTRA’S ALGORITHM 
int minDistance(int dist[], int sptSet[]) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

void printPath(int parent[], int j) {
    if (parent[j] == -1)
        return;
    printPath(parent, parent[j]);
    printf(" -> %s", cities[j]);
}

void Dijkstra(int src, int dest) {
    int dist[V], sptSet[V], parent[V];
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        if (u == -1) break;
        sptSet[u] = 1;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }

    printf("\n Fastest Delivery Route (Dijkstra’s Algorithm):\n");
    printf("------------------------------------------------\n");
    printf(" From %s to %s: %d km\n", cities[src], cities[dest], dist[dest]);
    printf(" Route: %s", cities[src]);
    printPath(parent, dest);
    printf("\n------------------------------------------------\n");
}

// FRACTIONAL KNAPSACK
void fractionalKnapsack(int n, float weight[], float profit[], float capacity) {
    float ratio[MAX], totalProfit = 0, cap = capacity;
    int i, j;

    for (i = 0; i < n; i++)
        ratio[i] = profit[i] / weight[i];

    // Sort by ratio descending
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (ratio[j] < ratio[j + 1]) {
                float temp = ratio[j];
                ratio[j] = ratio[j + 1];
                ratio[j + 1] = temp;
                temp = weight[j];
                weight[j] = weight[j + 1];
                weight[j + 1] = temp;
                temp = profit[j];
                profit[j] = profit[j + 1];
                profit[j + 1] = temp;
            }

    printf("\n Delivery Load Optimization (Fractional Knapsack):\n");
    printf("----------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        if (weight[i] <= cap) {
            totalProfit += profit[i];
            cap -= weight[i];
            printf(" Full parcel taken (Wt: %.1f, Profit: %.1f)\n", weight[i], profit[i]);
        } else {
            totalProfit += ratio[i] * cap;
            printf(" Partial parcel taken (Wt: %.1f, Profit: %.1f)\n", cap, ratio[i] * cap);
            break;
        }
    }
    printf("----------------------------------------------------\n");
    printf(" Maximum Delivery Value: %.2f\n", totalProfit);
}

//  MAIN FUNCTION 
int main() {
    printf("Enter number of cities: ");
    scanf("%d", &V);
    getchar();

    for (int i = 0; i < V; i++) {
        printf("Enter name of city %d: ", i + 1);
        fgets(cities[i], 50, stdin);
        trimNewline(cities[i]);
    }

    printf("Enter number of roads: ");
    scanf("%d", &E);
    struct Edge *edges = (struct Edge *)malloc(E * sizeof(struct Edge));

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            graph[i][j] = 0;

    printf("\nEnter road connections and distances:\n");
    for (int i = 0; i < E; i++) {
        char city1[50], city2[50];
        int w;
        getchar();
        printf("\nRoad %d:\n", i + 1);
        printf(" From city: ");
        fgets(city1, 50, stdin);
        trimNewline(city1);
        printf(" To city: ");
        fgets(city2, 50, stdin);
        trimNewline(city2);
        printf(" Distance (in km): ");
        scanf("%d", &w);

        int src = -1, dest = -1;
        for (int j = 0; j < V; j++) {
            if (strcmp(cities[j], city1) == 0) src = j;
            if (strcmp(cities[j], city2) == 0) dest = j;
        }
        edges[i].src = src;
        edges[i].dest = dest;
        edges[i].weight = w;
        graph[src][dest] = w;
        graph[dest][src] = w;
    }

    KruskalMST(edges, V, E);

    char source[50], destination[50];
    getchar();
    printf("\nEnter source city for delivery: ");
    fgets(source, 50, stdin);
    trimNewline(source);
    printf("Enter destination city: ");
    fgets(destination, 50, stdin);
    trimNewline(destination);

    int srcIndex = -1, destIndex = -1;
    for (int i = 0; i < V; i++) {
        if (strcmp(cities[i], source) == 0) srcIndex = i;
        if (strcmp(cities[i], destination) == 0) destIndex = i;
    }

    Dijkstra(srcIndex, destIndex);

    //  Fractional Knapsack Input 
    int n;
    float capacity;
    printf("\nEnter number of parcels: ");
    scanf("%d", &n);
    float weight[n], profit[n];

    for (int i = 0; i < n; i++) {
        printf(" Parcel %d weight (kg): ", i + 1);
        scanf("%f", &weight[i]);
        printf(" Parcel %d profit (₹): ", i + 1);
        scanf("%f", &profit[i]);
    }
    printf("Enter vehicle capacity (kg): ");
    scanf("%f", &capacity);

    fractionalKnapsack(n, weight, profit, capacity);

    free(edges);
    return 0;

}
