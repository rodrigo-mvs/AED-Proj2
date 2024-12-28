//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name : Rodrigo Santos
// Student Number : 119198
// Student Name : Gonçalo Ribau
// Student Number : 119560

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
  assert(g != NULL);

  // COMPLETE THE CODE
  // CREATE AUXILIARY (static) FUNCTIONS, IF USEFUL
  // Graph radius --- the smallest vertex eccentricity value
  // Graph diameter --- the largest vertex eccentricity value
  // Do not forget that -1 represents an IDEFINITE value

  // Computing the set of central vertices
  // Allocate the central vertices array : number of central vertices + 1
  // Fill in the central vertices array

  // Get the number of vertices in the given graph
  unsigned int numVertices = GraphGetNumVertices(g);

  // Allocate memory for the struct
  GraphEccentricityMeasures* emresult = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  assert(emresult != NULL);

  // Save the given graph on the struct
  emresult->graph = g;

  // Setting the initial values of both the diameter and the radius to infinite or undefinded (-1);
  emresult->graphRadius = -1;
  emresult->graphDiameter = -1;

  // Allocate memory for the eccentricity array
  emresult->eccentricity = (int*)malloc(numVertices * sizeof(int));
  assert(emresult->eccentricity != NULL);

  // Get the shortest distances between the vertices 
  GraphAllPairsShortestDistances* distances = GraphAllPairsShortestDistancesExecute(g);

  // First iteration through the vertices
  for (unsigned int v = 0; v < numVertices; v++) {
    int maxDistance = -1; // Initial distance (infinite)
    // Second iteration to form the pairs
    for (unsigned int w = 0; w < numVertices; w++) {
      int distance = GraphGetDistanceVW(distances, v, w); // Get the distance between both vertices
      if (distance != -1 && distance > maxDistance) {
        maxDistance = distance; // Update the max distance
      }
    }

    // Update de eccentricity of the vertex v
    emresult->eccentricity[v] = maxDistance;

    // Update the radius and the diameter 
    if (emresult->graphRadius == -1 || maxDistance < emresult->graphRadius) {
      emresult->graphRadius = maxDistance; // if the maxDistance is shorter than the radius, it turns into the new radius
    }
    if (maxDistance > emresult->graphDiameter) {
      emresult->graphDiameter = maxDistance; // if the maxDistance is longer than the diameter, it turns into the new diameter
    }
  }

  // Get central vertices
  unsigned int centralCount = 0; // Variable to count the number of vertices
  // Cycle through the vertices
  for (unsigned int v = 0; v < numVertices; v++) {
    if (emresult->eccentricity[v] == emresult->graphRadius) {
      centralCount++; // Add 1 for each central vertex identified
    }
  }

  // Allocation of memory for the central vertices array
  emresult->centralVertices = (unsigned int*)malloc((centralCount + 1) * sizeof(unsigned int));
  assert(emresult->centralVertices != NULL);
  // Number of vertices goes on index 0 of the array
  emresult->centralVertices[0] = centralCount;

  unsigned int index = 1; // index number for iteration 
  for (unsigned int v = 0; v < numVertices; v++) {
    if (emresult->eccentricity[v] == emresult->graphRadius) {
      emresult->centralVertices[index++] = v; // If the eccentricity of the vertex v is equal to the radius, the vertex is stored in the array as a central vertex
    }
  }
  // Destroy the temporary graph to free up memory
  GraphAllPairsShortestDistancesDestroy(&distances);

  return emresult;
}

void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
  assert(p != NULL);
  assert(p->centralVertices != NULL);

  // COMPLETE THE CODE

  unsigned int centralCount = p->centralVertices[0]; // Store the number of central vertices
  // Allocate memory for an array which will serve as a copy of the centralVertices array
  unsigned int* centralCopy = (unsigned int*)malloc((centralCount + 1) * sizeof(unsigned int));
  assert(centralCopy != NULL);

  // Iterate through the number of central vertices
  for (unsigned int i = 0; i <= centralCount; i++) {
    centralCopy[i] = p->centralVertices[i]; // Store the values of the central Vertices on this new array
  }

  return centralCopy;
}

// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  // COMPLETE THE CODE

  assert(p != NULL);

  // Print of the radius and diameter of the given graph
  printf("Graph radius: %d\n", p->graphRadius);
  printf("Graph diameter: %d\n", p->graphDiameter);

  // Print of the eccentricity values
  printf("Vertex eccentricities:\n");
  unsigned int numVertices = GraphGetNumVertices(p->graph); // Get the number of vertices in the given graph
  // Iterate through the vertices
  for (unsigned int v = 0; v < numVertices; v++) {
    printf("Vertex %u: %d\n", v, p->eccentricity[v]); // Print the vertex and its eccentricity value
  }

  printf("Number of Central vertices: %u\n", p->centralVertices[0]);
  printf("Central vertices: ");
  // Cycle through the centralVertices array starting on the element 1
  for (unsigned int i = 1; i <= p->centralVertices[0]; i++) {
    printf("%u ", p->centralVertices[i]); // Print each central vertex
  }
  printf("\n");
}
