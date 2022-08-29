#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#ifndef VETORINT_H
    #include "vetorInt.h"
#endif

typedef struct {
    int id_vertice_01;
    int id_vertice_02;
    float peso;
} Aresta;

typedef struct {
    int id_vertice;
    Aresta *arestas;
    int numero_arestas;
} Vertice;

typedef struct {
    Vertice *vertices;
    int numero_vertices;
} Grafo;

// Funções de grafo
Grafo *criaGrafo();
Grafo *lerDeArquivo(char *nome_arquivo, char *delimitador);
void liberaGrafo(Grafo *grafo_liberar);
void imprimeGrafo(Grafo *grafo_imprimir);
bool adicionarVertice(Grafo *grafo, Vertice vertice);
bool removerVertice(Grafo *grafo, Vertice vertice);
bool removerVerticePorId(Grafo *grafo, int id_vertice);
int procurarVerticePorID(Grafo *grafo_procurar, int id_vertice);
float pesoTotalGrafo(Grafo *grafo);

// Funções de Vértice
Vertice criaVertice(int id_vertice);
void liberaVertice(Vertice *vertice_liberar);
void imprimeVertice(Vertice vertice_imprimir);
bool adicionarAresta(Vertice *vertice, Aresta aresta);
bool removerAresta(Vertice *vertice, Aresta aresta);
bool removerArestaPorId(Vertice *vertice, int id_vertice_02);
bool verticeEhAdjacente(Vertice vertice_origem, Vertice vertice);
bool verticeEhAdjacentePorID(Vertice vertice_origem, int id_vertice);
int procurarAresta(Vertice vertice_origem, Vertice vertice_destino);
int procurarArestaPorID(Vertice vertice_origem, int id_vertice_destino);
void imprimirIDVertices(Grafo *grafo);
void copiarArestasVertice(Vertice *vertice_destino, Vertice *vertice_origem);
int grauDoVertice(Vertice vertice);

// Funções de Arestas (Conexão)
Aresta criaAresta(int id_vertice_01, int id_vertice_02, float peso);
void imprimeAresta(Aresta aresta_imprimir);

// Algoritmo de Kruskal
void quicksort(void *dado, int tamanho, int (*funcao_comparadora)(void *, void *), int tamanho_bytes);
int comparaPesoArestas(void *aresta1, void *aresta2);
Grafo *kruskal(Grafo *grafo);
bool conjuntosSeparados(Grafo **vetor_grafos, int tamanho, Vertice vertice01, Vertice vertice02);
int indiceGrafoContemVertice(Grafo **vetor_grafos, int tamanho, Vertice vertice);
int indiceGrafoContemVerticePorID(Grafo **vetor_grafos, int tamanho, int id_vertice);


// Algoritmo de Christofides
Grafo *subgrafoVerticesGrauImparSemArestas(Grafo *grafo);
Grafo *subgrafoVerticesGrauImpar(Grafo *grafo);
int numeroArestasNoGrafo(Grafo *grafo);
VetorInt visitarVertices(Grafo *grafo, Vertice vertice);
void _visitarVertices(Grafo *grafo, Vertice vertice, int id_vertice_pai, VetorInt *vetor_salvar);
Grafo *recriaGrafo(Grafo *grafo_original, VetorInt *sequencia_vertices);


#endif 