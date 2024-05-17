#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>



#define MAX_NODES 696  // Maximum No of Nodes in the graph
#define MAX_LINE_LENGTH 100
#define INF INT_MAX

typedef struct {
    char name[MAX_LINE_LENGTH];
} Node;

typedef struct {
    int numNodes;
    Node nodes[MAX_NODES];
    int adjacencyMatrix[MAX_NODES][MAX_NODES];
} Graph;

void initializeGraph(Graph* graph) 
{
    graph->numNodes = 0;
    for (int i = 0; i < MAX_NODES; i++) 
    {
        for (int j = 0; j < MAX_NODES; j++) 
        {
            graph->adjacencyMatrix[i][j] = 0;
        }
    }
}

void readGraphFromCSV(const char* filename, Graph* graph) 
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char source[MAX_LINE_LENGTH], target[MAX_LINE_LENGTH], weightStr[MAX_LINE_LENGTH];
        int weight;
        if (sscanf(line, "%[^,],%[^,],%s", source, target, weightStr) != 3) 
        {
            fprintf(stderr, "Error: Invalid data format in file %s\n", filename);
            exit(1);
        }
        weight = atoi(weightStr);

        int sourceIndex = -1, targetIndex = -1;
        for (int i = 0; i < graph->numNodes; i++) 
        {
            if (strcmp(graph->nodes[i].name, source) == 0) 
            {
                sourceIndex = i;
                break;
            }
        }
        if (sourceIndex == -1 && graph->numNodes < MAX_NODES) 
        {
            strcpy(graph->nodes[graph->numNodes].name, source);
            sourceIndex = graph->numNodes;
            graph->numNodes++;
        }

        for (int i = 0; i < graph->numNodes; i++) {
            if (strcmp(graph->nodes[i].name, target) == 0) 
            {
                targetIndex = i;
                break;
            }
        }
        if (targetIndex == -1 && graph->numNodes < MAX_NODES) 
        {
            strcpy(graph->nodes[graph->numNodes].name, target);
            targetIndex = graph->numNodes;
            graph->numNodes++;
        }

        if (sourceIndex != -1 && targetIndex != -1) 
        {
            graph->adjacencyMatrix[sourceIndex][targetIndex] = weight;
            graph->adjacencyMatrix[targetIndex][sourceIndex] = weight;
        } 
        else 
        {
            fprintf(stderr, "Error: Maximum number of nodes reached\n");
            exit(1);
        }
    }

    fclose(file);
}

void computeKShortestPaths(const Graph* graph, int k) 
{
    int (*dist)[MAX_NODES] = malloc(MAX_NODES * sizeof(int[MAX_NODES]));
    int (*path)[MAX_NODES] = malloc(MAX_NODES * sizeof(int[MAX_NODES]));
    if (dist == NULL || path == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < graph->numNodes; i++) 
    {
        for (int j = 0; j < graph->numNodes; j++) 
        {
            if (i == j) 
            {
                dist[i][j] = 0;
            } 
            else if (graph->adjacencyMatrix[i][j] != 0) 
            {
                dist[i][j] = graph->adjacencyMatrix[i][j];
            } 
            else 
            {
                dist[i][j] = INF;
            }
            if (dist[i][j] != INF) 
            {
                path[i][j] = j;
            } 
            else 
            {
                path[i][j] = -1;
            }
        }
    }

    srand(time(NULL));
    int selectedNodes[k][2];
    for (int i = 0; i < k; i++) 
    {
        selectedNodes[i][0] = rand() % graph->numNodes; // Source node
        selectedNodes[i][1] = rand() % graph->numNodes; // Destination node
    }
    
    for (int l = 0; l < graph->numNodes; l++) 
    {
        for (int i = 0; i < graph->numNodes; i++) 
        {
            for (int j = 0; j < graph->numNodes; j++) 
            {
                if (dist[i][l] != INF && dist[l][j] != INF && dist[i][l] + dist[l][j] < dist[i][j]) 
                {
                    dist[i][j] = dist[i][l] + dist[l][j];
                    path[i][j] = path[i][l]; 
                }
            }
        }
    }

    FILE* outFile = fopen("shortest_paths.csv", "w");
    if (outFile == NULL) 
    {
        fprintf(stderr, "Error: Unable to create output file\n");
        exit(1);
    }
    for (int i = 0; i < k; i++) 
    {
        int source = selectedNodes[i][0];
        int destination = selectedNodes[i][1];
        if (dist[source][destination] != INF) 
        {
            fprintf(outFile, "Shortest Path from %s to %s: %d\n", graph->nodes[source].name, graph->nodes[destination].name, dist[source][destination]);
            fprintf(outFile, "%s", graph->nodes[source].name);
            int node = source;
            while (node != destination) 
            {
                node = path[node][destination];
                fprintf(outFile, " -> %s", graph->nodes[node].name);
            }
            fprintf(outFile, "\n\n");
        }
    }
    fclose(outFile);
    free(dist);
    free(path);
}
double computeExecutionTime(clock_t start, clock_t end) {
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}
int main() 
{
    const char* filename = "doctorwho.csv";
    Graph graph;
    clock_t start = clock();
    initializeGraph(&graph);
    readGraphFromCSV(filename, &graph);
    int k = 10;
    computeKShortestPaths(&graph, k);
    clock_t end = clock();

    double time_taken = computeExecutionTime(start, end);
    printf("Execution Time: %f seconds\n", time_taken);

    return 0;
}

