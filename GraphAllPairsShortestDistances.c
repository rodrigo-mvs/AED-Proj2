//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphAllPairsShortestDistances
//

// Student Name : Rodrigo Santos
// Student Number : 119198
// Student Name : Gonçalo Ribau
// Student Number : 119560

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

  // Get the number of vertices of the given Graph
  unsigned int numVertices = GraphGetNumVertices(g);

  // Allocatio of memory for the new temporary Graph struct we will use
  GraphAllPairsShortestDistances* apsdresult = (GraphAllPairsShortestDistances*)malloc(sizeof(GraphAllPairsShortestDistances)); // Graph

  // Allocation of memory for the distance 2D array
  apsdresult->distance = (int**)malloc(numVertices * sizeof(int*));
  // Allocation for each row individually
  for (unsigned int i = 0; i < numVertices; i++) {
    apsdresult->distance[i] = (int*)malloc(numVertices * sizeof(int));
    // Setting the initial value for each element
    for (unsigned int j = 0; j < numVertices; j++) {
      apsdresult->distance[i][j] = -1; // Initially the value is not defined (infinite or "-1" in this case)
    }
  }

  // Store the graph
  apsdresult->graph = g;

  // Iterate through all the graph's vertices
  for (unsigned int u = 0; u < numVertices; u++) {
    GraphBellmanFordAlg* bfgraph = GraphBellmanFordAlgExecute(g, u); // Do the Belman-Ford algorythm for the first vertice to find paths
    // Iterate again through the vertices to check the pairs
    for (unsigned int v = 0; v < numVertices; v++) {
      if (GraphBellmanFordAlgPathTo(bfgraph, v)) {
        apsdresult->distance[u][v] = GraphBellmanFordAlgDistance(bfgraph, v);
      }
    }
    // Destroy the temporary struct to improve memory efficiency
    GraphBellmanFordAlgDestroy(&bfgraph);
  }

  return apsdresult;
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
