//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name : Gonçalo Ribau
// Student Number : 119560
// Student Name :
// Student Number :

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g));
  assert(GraphIsWeighted(g) == 0);

  // COMPLETE THE CODE

  // Get the number of vertices on the given graph
  unsigned int numVertices = GraphGetNumVertices(g);
  // Create the resulting graph
  Graph* tcresult = GraphCreate(numVertices, 1, 0); // Directed and unweighted

  // Cycle through all the available vertices
  for (unsigned int u = 0; u < numVertices; u++) {
    // Run Bellman-Ford on the given graph with u as strating point
    GraphBellmanFordAlg* bfresult = GraphBellmanFordAlgExecute(g, u); // This checks which vertices are connected to u

    // Cycle through all the available vertices once more
    for (unsigned int v = 0; v < numVertices; v++) {
      if (u != v && GraphBellmanFordAlgReached(bfresult, v)) // If we are able to get path information on this vertix

      GraphAddEdge(tcresult, u, v); // Add the respective edge to the Transitive Closer Graph
    }

    // Destroy the temporary Belman-Ford graph to free up memory
    GraphBellmanFordAlgDestroy(&bfresult);
  }

  return tcresult;
}
