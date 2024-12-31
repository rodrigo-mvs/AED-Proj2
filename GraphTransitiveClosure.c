//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name : Rodrigo Santos
// Student Number : 119198
// Student Name : Gonçalo Ribau
// Student Number : 119560

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

void GraphTCInit(void) {  ///
  InstrCalibrate();
  InstrName[3] = "bf_invocation_counter"; 
  InstrName[4] = "edge_addition_counter";
}

#define BF_INVOCATION_COUNTER InstrCount[3]
#define EDGE_ADDITION_COUNTER InstrCount[4]


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
    BF_INVOCATION_COUNTER++;// count how many times the Bellman-Ford module is called

    // Cycle through all the available vertices once more
    for (unsigned int v = 0; v < numVertices; v++) {
      if (u != v && GraphBellmanFordAlgReached(bfresult, v)) { // If we are able to get path information on this vertix

        EDGE_ADDITION_COUNTER++; // Count how many times an edge is added to the graph
        GraphAddEdge(tcresult, u, v); // Add the respective edge to the Transitive Closer Graph
      }
    }
    // Destroy the temporary struct to improve memory efficiency
    GraphBellmanFordAlgDestroy(&bfresult);
  }

  return tcresult;
}
