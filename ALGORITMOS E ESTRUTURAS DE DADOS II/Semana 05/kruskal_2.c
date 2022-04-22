#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Estruturas do programa
typedef struct {
    int id_vertice_01;
    int id_vertice_02;
    float peso;
} Aresta;

typedef struct {
    int id_vertice;
    Aresta *arestas;
    int numero_conexoes;
} Vertice;

typedef struct {
    Vertice *vertices;
    int numero_vertices;
} Grafo;


// Funções de grafo
Grafo *criaGrafo();
void liberaGrafo(Grafo *grafo_liberar);
void imprimeGrafo(Grafo *grafo_imprimir);
void adicionarVertice(Grafo *grafo, Vertice vertice);
int procurarVerticePorID(Grafo *grafo_procurar, int id_vertice);

// Funções de Vértice
Vertice criaVertice(int id_vertice);
void liberaVertice(Vertice vertice_liberar);
void imprimeVertice(Vertice vertice_imprimir);
void adicionarAresta(Vertice *vertice, Aresta aresta);
bool verticeEhAdjacente(Vertice vertice_origem, Vertice vertice);
bool verticeEhAdjacentePorID(Vertice vertice_origem, int id_vertice);

// Funções de Arestas (Conexão)
Aresta criaAresta(int id_vertice_01, int id_vertice_02, float peso);
void imprimeAresta(Aresta aresta_imprimir);

// Funções do programa
typedef enum {INSERIR_VERTICE=1, MOSTRAR_GRAFO, ALTERAR_ARESTA, GERAR_MST, SAIR} OPCAO_MENU;
OPCAO_MENU menu();
int lerNumeroInteiro();
float lerNumeroReal();

void insereVertice(Grafo *grafo);
void mostrarGrafo(Grafo *grafo);
void alterarAresta(Grafo *grafo);
void gerarMST(Grafo *grafo);
void sair(Grafo *grafo);


int main() {
    Grafo *grafo = criaGrafo();
    if (grafo == NULL) {
        exit(EXIT_FAILURE);
    }

    OPCAO_MENU escolha_usuario;
    while (true){
        escolha_usuario = menu();

        switch (escolha_usuario)
        {
        case INSERIR_VERTICE:
            /* code */
            break;
        case MOSTRAR_GRAFO:
            break;
        case ALTERAR_ARESTA:
            break;
        case GERAR_MST:
            break;
        case SAIR:
            break;
        default:
            printf("\nOPCAO ERRADA");
            break;
        }

    }

    return EXIT_SUCCESS;
}

// Funções de grafo
Grafo *criaGrafo(){
    Grafo *temp = (Grafo *) malloc (sizeof(Grafo));
    if (temp == NULL){
        printf("\nErro ao criar grafo\n");
        return NULL;
    }

    temp->numero_vertices = 0;
    temp->vertices = NULL;

    return temp;
}

void liberaGrafo(Grafo *grafo_liberar){
    if (grafo_liberar){
        if (grafo_liberar->vertices) {
            for (int c = 0; c < grafo_liberar->numero_vertices; c++){
                liberaVertice(grafo_liberar->vertices[c]);
            }
            free(grafo_liberar->vertices);
        }
        free(grafo_liberar);
    }
}

void imprimeGrafo(Grafo *grafo_imprimir){
    if (grafo_imprimir) {
        for (int c = 0; c < grafo_imprimir->numero_vertices; c++){
            printf("\n");
            imprimeVertice(grafo_imprimir->vertices[c]);
        }
        printf("\n");
    }
}
void adicionarVertice(Grafo *grafo, Vertice vertice){
    if (grafo) {
        if (procurarVerticePorID(grafo, vertice.id_vertice) == -1){
            // Aumenta o vetor que guarda os Vértices
            grafo->numero_vertices++;
            grafo->vertices = (Vertice *) realloc(grafo->vertices, sizeof(Vertice) * grafo->numero_vertices);
            if (grafo->vertices == NULL) {
                printf("\nErro ao adicionar Vertice");
                grafo->numero_vertices = 0;
                return;
            }

            grafo->vertices[grafo->numero_vertices - 1] = vertice;
        }
        else {
            printf("\nVertice com o id %d ja existe no grafo.", vertice.id_vertice);
        }
    }
}
int procurarVerticePorID(Grafo *grafo_procurar, int id_vertice){
    if (grafo_procurar){
        for (int c = 0; c < grafo_procurar->numero_vertices; c++) {
            if (grafo_procurar->vertices[c].id_vertice == id_vertice) {
                return c;
            }
        }
    }
    return -1;
}

// Funções de Vértice
Vertice criaVertice(int id_vertice){
    Vertice temp = {.id_vertice = id_vertice,
                    .arestas = NULL,
                    .numero_conexoes = 0};
    return temp;
}
void liberaVertice(Vertice vertice_liberar) {
    if (vertice_liberar.arestas) {
        free(vertice_liberar.arestas);
    }
}
void imprimeVertice(Vertice vertice_imprimir){
    printf("\n");
    printf("ID do Vertice: %d\n", vertice_imprimir.id_vertice);
    printf("Numero de arestas %d", vertice_imprimir.numero_conexoes);
    for (int c = 0; c < vertice_imprimir.numero_conexoes; c++){
        printf("\n\t");
        imprimeAresta(vertice_imprimir.arestas[c]);
    }
}
void adicionarAresta(Vertice *vertice, Aresta aresta){
    if (aresta.id_vertice_01 == vertice->id_vertice){
        vertice->numero_conexoes++;
        vertice->arestas = realloc(vertice->arestas, sizeof(Aresta) * vertice->numero_conexoes);
        if (vertice->arestas == NULL){
            printf("\nErro ao adicionar aresta");
            vertice->numero_conexoes = 0;
            return;
        }

        vertice->arestas[vertice->numero_conexoes - 1] = aresta;
    }
    else {
        printf("\nNao e possivel adicionar a aresta ao vertice, id_vertice_01 e diferente do id do vertice");
    }

}
bool verticeEhAdjacente(Vertice vertice_origem, Vertice vertice);
bool verticeEhAdjacentePorID(Vertice vertice_origem, int id_vertice);

// Funções de Arestas (Conexão)
Aresta criaAresta(int id_vertice_01, int id_vertice_02, float peso);
void imprimeAresta(Aresta aresta_imprimir){
    printf("(%d,%d): %.2f", aresta_imprimir.id_vertice_01, aresta_imprimir.id_vertice_02, aresta_imprimir.peso);
}

// Funções do programa
void insereVertice(Grafo *grafo);
void mostrarGrafo(Grafo *grafo);
void alterarAresta(Grafo *grafo);
void gerarMST(Grafo *grafo);
void sair(Grafo *grafo);

int lerNumeroInteiro(){
    int numero = 0;
    while (scanf("%d%*c", &numero) == 0) {
        while (getc(stdin) != '\n');
    }
    return numero;
}

float lerNumeroReal(){
    float numero = 0;
    while (scanf("%f%*c", &numero) == 0) {
        while (getc(stdin) != '\n');
    }
    return numero;
}

OPCAO_MENU menu(){
    OPCAO_MENU escolha;
    do {
        printf("\n");
        printf("\n\t%d INSERIR VERTICE", INSERIR_VERTICE);
        printf("\n\t%d MOSTRAR GRAFO", MOSTRAR_GRAFO);
        printf("\n\t%d ALTERAR ARESTA", ALTERAR_ARESTA);
        printf("\n\t%d MOSTRAR ARVORE GERADORA MINIMA", GERAR_MST);
        printf("\n\t%d SAIR", SAIR);
        escolha = lerNumeroInteiro();
    } while (escolha != INSERIR_VERTICE &&
             escolha != MOSTRAR_GRAFO &&
             escolha != ALTERAR_ARESTA && 
             escolha !=  GERAR_MST &&
             escolha != SAIR
    );
    return escolha;
}