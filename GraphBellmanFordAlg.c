//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name : Rodrigo Santos
// Student Number : 119198
// Student Name : Gonçalo Ribau
// Student Number : 119560

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"

void GraphBFInit(void) {  ///
  InstrCalibrate();
  InstrName[0] = "Initialization_counter"; 
  InstrName[1] = "edge_comparison_counter";
  InstrName[2] = "relaxation_counter";
}

// Global counters to acess the Complexity
#define INITIALIZATION_COUNTER InstrCount[0]
#define EDGE_COMPARISON_COUNTER InstrCount[1]
#define RELAXATION_COUNTER InstrCount[2]


struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g,
                                                unsigned int startVertex) {
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);

  unsigned int numVertices = GraphGetNumVertices(g); // Get the number of vertices in the given graph 

  GraphBellmanFordAlg* result = (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  // Given graph and start vertex for the shortest-paths
  result->graph = g;
  result->startVertex = startVertex;

  //
  // TO BE COMPLETED !!
  //
  // CREATE AND INITIALIZE
  // result->marked
  // result->distance
  // result->predecessor
  //

  // Mark all vertices as not yet visited, i.e., ZERO
  
  // No vertex has (yet) a (valid) predecessor
  
  // No vertex has (yet) a (valid) distance to the start vertex
  
  // THE ALGORTIHM TO BUILD THE SHORTEST-PATHS TREE

  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  assert(result->marked != NULL);

  result->distance = (int*)malloc(numVertices * sizeof(int));
  assert(result->distance != NULL);

  result->predecessor = (int*)malloc(numVertices * sizeof(int));
  assert(result->predecessor != NULL);

  // Step 1 : Initialize graph
  for (unsigned int i = 0; i < numVertices; i++) {
    INITIALIZATION_COUNTER++; // Count the number of iterations in the initialization

    result->marked[i] = 0;
    result->distance[i] = -1;
    result->predecessor[i] = -1;
  }

  // The distance on the source is 0
  result->distance[startVertex] = 0;
  result->marked[startVertex] = 1;


  // Step 2 : Relax edges repeatedly
  // Relax all edges |V|-1 times
  for (unsigned int i = 0; i < numVertices - 1; i++) {
    // For each edge
    for (unsigned int v = 0; v < numVertices; v++) {

      if (result->marked[v] == 0) {
        continue;
      }

      unsigned int* adjacents = GraphGetAdjacentsTo(g, v);  // Get adjacent vertices for edge v

      if (adjacents == NULL) {
        fprintf(stderr, "Error: adjacents is NULL for edge %u\n", v);
        exit(EXIT_FAILURE);
      }

      // For each adjacent vertex, try relaxing the edge
      for (unsigned int j = 1; j <= adjacents[0]; j++) {
        EDGE_COMPARISON_COUNTER++; // Count the comparison of the edges

        unsigned int adj = adjacents[j];  // Adjacent vertex

        // Relaxation condition: if the distance to the adjacent vertex can be reduced
        if (result->distance[adj] == -1 || result->distance[v] + 1 < result->distance[adj]) {
          RELAXATION_COUNTER++; // Count the number of relaxations

          result->marked[adj] = 1;
          result->distance[adj] = result->distance[v] + 1;  // Relax edge
          result->predecessor[adj] = v;  // Set predecessor for path reconstruction
        }
      }
      free(adjacents);
    }
  }

  // Step 3: Negative cycle detection is skipped since the graph is unweighted (no negative weights)

  return result;
}

void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(*p != NULL);

  GraphBellmanFordAlg* aux = *p;

  free(aux->marked);
  free(aux->predecessor);
  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->distance[v];
}
Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {
    return s;
  }

  // Store the path
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // The paths tree is a digraph, with no edge weights
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use the predecessors array to add the tree edges
  for (unsigned int w = 0; w < num_vertices; w++) {
    // Vertex w has a predecessor vertex v?
    int v = p->predecessor[w];
    if (v != -1) {
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Display the tree in the DOT format
  GraphDisplayDOT(paths_tree);

  // Housekeeping
  GraphDestroy(&paths_tree);
}
