// Test file for the simulations used on the report
//
// Utilizar o comando "make TestSimulationBFTC" para compilar
// Caso dê problemas remover dos "TARGETS" do Makefile

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "GraphTransitiveClosure.h"
#include "instrumentation.h"


int edgeExists(Graph* graph, unsigned int v, unsigned int w) {
    unsigned int* adjacents = GraphGetAdjacentsTo(graph, v);

    // printf("vertex: %u\n", v);

    for (unsigned int i = 1; i <= adjacents[0]; i++) {

        // printf("number: %u\n", adjacents[0]);
        // printf("element: %u\n", adjacents[i]);
        // printf("w: %u\n", w);

        if (adjacents[i] == w) {
            // printf("exists\n");
            // printf("\n");
            return 1;
        }
    }
    free(adjacents);

    return 0;
}


Graph* generateRandomGraph(unsigned int numVertices, unsigned int numEdges, int isDigraph) {
    Graph* graph = GraphCreate(numVertices, isDigraph, 0);
    unsigned int v, w;

    unsigned int edgeCounter = 0;

    while (edgeCounter < numEdges) {
        do {
            v = rand() % numVertices;
            w = rand() % numVertices;
        } while (v == w);

        if (!edgeExists(graph, v, w)){
            // printf("adding edge\n");
            // printf("\n");
            GraphAddEdge(graph, v, w);

            edgeCounter++;
        }
    }

    return graph;
}


int main(void) {
    // Importante para o rand()
    srand(time(0));

    // Inicializar os counters
    GraphBFInit();
    GraphTCInit();

    // Variáveis iniciais
    int isDigraph = 1; // Se quisermos mudar para um grafo sem direções
    unsigned int numVertices = 2; // Número de vértices inicial/atual
    unsigned int fnumVertices = 128; // Número de vértices final

    while (numVertices <= fnumVertices) {

        int numEdges = numVertices - 1; // Número de edges (podemos aumentar caso seja preciso um grafo mais denso)

        Graph* graph_sim = generateRandomGraph(numVertices, numEdges, isDigraph);

        // GraphDisplayDOT(graph_sim); // Só para ver se o grafo está a ser bem criado

        // ---------- Funções a testar -----------
        //
        // Warning: Testar uma de cada vez!
        // (comentar a não utilizada)
        //
        // ----- Bellman-Ford:
        

        for (unsigned int v = 0; v < numVertices; v++) {
            GraphBellmanFordAlg* BF_sim = GraphBellmanFordAlgExecute(graph_sim, v);

            GraphBellmanFordAlgDestroy(&BF_sim);
        }
        printf("Num. Vertices: %u\n", numVertices);
        printf("Iterations: %ld\n", InstrCount[0] + InstrCount[1] + InstrCount[6]);
        printf("\n");
        
        // reset dos counters
        InstrCount[0] = 0;
        InstrCount[1] = 0;
        InstrCount[6] = 0;
        

        //
        //
        // ----- Transitive Closure:
        

        // Graph* TC_sim = GraphComputeTransitiveClosure(graph_sim);
        // GraphDestroy(&TC_sim);
        // printf("Num. Vertices: %u\n", numVertices);
        // printf("Iterations: %ld\n", InstrCount[0] + InstrCount[1] + InstrCount[3] + InstrCount[4] + InstrCount[5] + InstrCount[6]);
        // printf("\n");
        
        // // reset dos counters
        // InstrCount[0] = 0;
        // InstrCount[1] = 0;
        // InstrCount[3] = 0;
        // InstrCount[4] = 0;
        // InstrCount[5] = 0;
        // InstrCount[6] = 0;


        //
        // ---------- Fim dos Testes -----------

        GraphDestroy(&graph_sim);
        numVertices *= 2; // De que forma queremos incrementar o numVertices
    }
    return 0;
}
