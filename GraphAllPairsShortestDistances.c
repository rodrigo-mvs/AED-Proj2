//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphAllPairsShortestDistances
//

// Student Name : Gonçalo Ribau
// Student Number : 119560
// Student Name :
// Student Number :

/*** COMPLETE THE GraphAllPairsShortestDistancesExecute FUNCTION ***/

#include "GraphAllPairsShortestDistances.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"

struct _GraphAllPairsShortestDistances {
  int** distance;  // The 2D matrix storing the all-pairs shortest distances
                   // It is stored as an array of pointers to 1D rows
                   // Idea: an INDEFINITE distance value is stored as -1
  Graph* graph;
};

// Allocate memory and initialize the distance matrix
// Compute the distances between vertices by running the Bellman-Ford algorithm
GraphAllPairsShortestDistances* GraphAllPairsShortestDistancesExecute(
    Graph* g) {
  assert(g != NULL);

  // COMPLETE THE CODE

  // get the number of vertices in the graph
  unsigned int vertices = GraphGetNumVertices(g);
  
  // Allocate memory for the distance matrix
  int** distances = (int**)malloc(vertices * sizeof(int*)); // for the 2D array in general
  for (unsigned int i = 0; i < vertices; i++) {
    distances[i] = (int*)malloc(vertices * sizeof(int)); // for each row
    for (unsigned int j = 0; j < vertices; j++) {
      distances[i][j] = -1;  // Initialize to -1 (indicating infinity)
    }
  }

  // Using Bellman-Ford to calculate the shortest paths
  for (unsigned int source = 0; source < vertices; source++) {
    // Arrays to store the distances and predecessors for the current source
    int* distance = (int*)malloc(vertices * sizeof(int));
    int* predecessor = (int*)malloc(vertices * sizeof(int));

    // Step 1: Initialize distances and predecessors
    for (unsigned int v = 0; v < vertices; v++) {
      distance[v] = -1;  // Distance is infinity initially
      predecessor[v] = -1;  // No predecessor initially
    }
    distance[source] = 0;  // Distance to the source is 0

    // Step 2: Relax all edges |V| - 1 times
    unsigned int numEdges = GraphGetNumEdges(g);
    for (unsigned int i = 0; i < vertices - 1; i++) {
      for (unsigned int v = 0; v < vertices; v++) {
        unsigned int* adjacents = GraphGetAdjacentsTo(g, v);
        double* edgeWeights = GraphGetDistancesToAdjacents(g, v);
        for (unsigned int j = 0; adjacents[j] != -1; j++) {
          unsigned int w = adjacents[j];
          double weight = edgeWeights[j];
          if (distance[v] != -1 && distance[v] + weight < distance[w]) {
            distance[w] = distance[v] + weight;
            predecessor[w] = v;
          }
        }
      }
    }

    // Step 3: Check for negative-weight cycles
    for (unsigned int v = 0; v < vertices; v++) {
      unsigned int* adjacents = GraphGetAdjacentsTo(g, v);
      double* edgeWeights = GraphGetDistancesToAdjacents(g, v);
      for (unsigned int j = 0; adjacents[j] != -1; j++) {
        unsigned int w = adjacents[j];
        double weight = edgeWeights[j];
        if (distance[v] != -1 && distance[v] + weight < distance[w]) {
          // Negative cycle detected, find the cycle
          int* visited = (int*)malloc(vertices * sizeof(int));
          for (unsigned int k = 0; k < vertices; k++) visited[k] = 0;
          
          unsigned int cycleStart = w;
          visited[cycleStart] = 1;
          unsigned int u = predecessor[cycleStart];
          
          while (!visited[u]) {
            visited[u] = 1;
            u = predecessor[u];
          }
          
          // If u has been visited, we found the cycle starting from u
          unsigned int cycleVertex = u;
          printf("Negative cycle detected: ");
          do {
            printf("%u ", cycleVertex);
            cycleVertex = predecessor[cycleVertex];
          } while (cycleVertex != u);
          
          free(visited);
          free(distance);
          free(predecessor);
          return NULL;  // Negative cycle found, terminate
        }
      }
    }

    // Store the computed distances for the current source vertex
    for (unsigned int v = 0; v < vertices; v++) {
      distances[source][v] = distance[v];
    }

    // Clean up the temporary arrays
    free(distance);
    free(predecessor);
  }

  // Create the result structure
  GraphAllPairsShortestDistances* result = (GraphAllPairsShortestDistances*)malloc(sizeof(GraphAllPairsShortestDistances));
  result->distance = distances;
  result->graph = g;

  return result;
}

void GraphAllPairsShortestDistancesDestroy(GraphAllPairsShortestDistances** p) {
  assert(*p != NULL);

  GraphAllPairsShortestDistances* aux = *p;
  unsigned int numVertices = GraphGetNumVertices(aux->graph);

  for (unsigned int i = 0; i < numVertices; i++) {
    free(aux->distance[i]);
  }

  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the result

int GraphGetDistanceVW(const GraphAllPairsShortestDistances* p, unsigned int v,
                       unsigned int w) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(w < GraphGetNumVertices(p->graph));

  return p->distance[v][w];
}

// DISPLAYING on the console

void GraphAllPairsShortestDistancesPrint(
    const GraphAllPairsShortestDistances* p) {
  assert(p != NULL);

  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Graph distance matrix - %u vertices\n", numVertices);

  for (unsigned int i = 0; i < numVertices; i++) {
    for (unsigned int j = 0; j < numVertices; j++) {
      int distanceIJ = p->distance[i][j];
      if (distanceIJ == -1) {
        // INFINITY - j was not reached from i
        printf(" INF");
      } else {
        printf(" %3d", distanceIJ);
      }
    }
    printf("\n");
  }
}
