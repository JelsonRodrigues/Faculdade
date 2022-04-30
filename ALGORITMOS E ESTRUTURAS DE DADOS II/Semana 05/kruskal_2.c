#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


// Estruturas do programa
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
void liberaGrafo(Grafo *grafo_liberar);
void imprimeGrafo(Grafo *grafo_imprimir);
void adicionarVertice(Grafo *grafo, Vertice vertice);
int procurarVerticePorID(Grafo *grafo_procurar, int id_vertice);

// Funções de Vértice
Vertice criaVertice(int id_vertice);
void liberaVertice(Vertice *vertice_liberar);
void imprimeVertice(Vertice vertice_imprimir);
void adicionarAresta(Vertice *vertice, Aresta aresta);
bool verticeEhAdjacente(Vertice vertice_origem, Vertice vertice);
bool verticeEhAdjacentePorID(Vertice vertice_origem, int id_vertice);
int procurarAresta(Vertice vertice_origem, Vertice vertice_destino);
int procurarArestaPorID(Vertice vertice_origem, int id_vertice_destino);
void imprimirIDVertices(Grafo *grafo);

// Funções de Arestas (Conexão)
Aresta criaAresta(int id_vertice_01, int id_vertice_02, float peso);
void imprimeAresta(Aresta aresta_imprimir);

// Funções do programa
typedef enum {INSERIR_VERTICE=1, MOSTRAR_GRAFO, INSERIR_ARESTA, ALTERAR_ARESTA, GERAR_MST, SAIR} OPCAO_MENU;
OPCAO_MENU menu();
int lerNumeroInteiro();
float lerNumeroReal();

// Opção do menu 
void insereVertice(Grafo *grafo);
void mostrarGrafo(Grafo *grafo);
void inserirAresta(Grafo *grafo);
void alterarAresta(Grafo *grafo);
void gerarMST(Grafo *grafo);
void sair(Grafo *grafo);


// Algoritmo de Kruskal
void quicksort(void *dado, int tamanho, int (*funcao_comparadora)(void *, void *), void (*funcao_swap)(void *, void *), int tamanho_bytes);
Grafo *kruskal(Grafo *grafo);
bool conjuntosSeparados(Grafo **vetor_grafos, int tamanho, Vertice vertice01, Vertice vertice02);
int indiceGrafoContemVertice(Grafo **vetor_grafos, int tamanho, Vertice vertice);

int main() {
    Grafo *grafo = criaGrafo();
    if (grafo == NULL) {
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    OPCAO_MENU escolha_usuario;
    while (true){
        escolha_usuario = menu();

        switch (escolha_usuario)
        {
        case INSERIR_VERTICE:
            insereVertice(grafo);
            break;
        case MOSTRAR_GRAFO:
            mostrarGrafo(grafo);
            break;
        case INSERIR_ARESTA:
            inserirAresta(grafo);
            break;
        case ALTERAR_ARESTA:
            alterarAresta(grafo);
            break;
        case GERAR_MST:
            gerarMST(grafo);
            break;
        case SAIR:
            sair(grafo);
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
                liberaVertice(&grafo_liberar->vertices[c]);
            }
            free(grafo_liberar->vertices);
            grafo_liberar->vertices = NULL;
            grafo_liberar->numero_vertices = 0;
        }
        free(grafo_liberar);
    }
}

void imprimeGrafo(Grafo *grafo_imprimir){
    if (grafo_imprimir) {
        printf("\nGrafo possui %d vertices", grafo_imprimir->numero_vertices);
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
                    .numero_arestas = 0};
    return temp;
}

void liberaVertice(Vertice *vertice_liberar) {
    if (vertice_liberar->arestas) {
        free(vertice_liberar->arestas);
        vertice_liberar->arestas = NULL;
        vertice_liberar->numero_arestas = 0;
    }
}

void imprimeVertice(Vertice vertice_imprimir){
    printf("\n");
    printf("ID do Vertice: %d\n", vertice_imprimir.id_vertice);
    printf("Numero de arestas %d", vertice_imprimir.numero_arestas);
    for (int c = 0; c < vertice_imprimir.numero_arestas; c++){
        printf("\n\t");
        imprimeAresta(vertice_imprimir.arestas[c]);
    }
}

void adicionarAresta(Vertice *vertice, Aresta aresta){
    if (vertice == NULL) {
        printf("\nVertice para adicionar invalido"); 
        return;
    }
    if (aresta.id_vertice_01 == vertice->id_vertice){
        vertice->numero_arestas++;
        vertice->arestas = (Aresta *) realloc(vertice->arestas, sizeof(Aresta) * vertice->numero_arestas);
        if (vertice->arestas == NULL){
            printf("\nErro ao adicionar aresta");
            vertice->numero_arestas = 0;
            return;
        }
        vertice->arestas[vertice->numero_arestas - 1] = aresta;
    }
    else {
        printf("\nNao e possivel adicionar a aresta ao vertice, id_vertice_01 e diferente do id do vertice");
    }
}

bool modificaArestaPorID(Vertice vertice_origem, int id_vertice_destino, float novo_valor){
    int posicao_aresta = procurarArestaPorID(vertice_origem, id_vertice_destino);
    if (posicao_aresta == -1) return false;
    vertice_origem.arestas[posicao_aresta].peso = novo_valor;
    return true; 
}

bool verticeEhAdjacente(Vertice vertice_origem, Vertice vertice){
    return verticeEhAdjacentePorID(vertice_origem, vertice.id_vertice);
}

bool verticeEhAdjacentePorID(Vertice vertice_origem, int id_vertice){
    return procurarArestaPorID(vertice_origem, id_vertice) != -1;
}

int procurarAresta(Vertice vertice_origem, Vertice vertice_destino){
    return procurarArestaPorID(vertice_origem, vertice_destino.id_vertice);
}
int procurarArestaPorID(Vertice vertice_origem, int id_vertice_destino){
    for (int c = 0; c < vertice_origem.numero_arestas; c++){
        if (vertice_origem.arestas[c].id_vertice_02 == id_vertice_destino) {
            return c;
        }
    }
    return -1;
}

// Funções de Arestas (Conexão)
Aresta criaAresta(int id_vertice_01, int id_vertice_02, float peso){
    Aresta aresta_nova = {.id_vertice_01 = id_vertice_01,
                          .id_vertice_02 = id_vertice_02,
                          .peso = peso};
    return aresta_nova;
}
void imprimeAresta(Aresta aresta_imprimir){
    printf("(%d,%d): %.2f", aresta_imprimir.id_vertice_01, aresta_imprimir.id_vertice_02, aresta_imprimir.peso);
}

// Funções do programa
void insereVertice(Grafo *grafo){
    if (grafo == NULL) return;
    printf("\nADICIONAR VÉRTICE");
    
    // Ler vértice
    int id_vertice;
    while (true){
        printf("\nDigite o ID do vértice: ");
        id_vertice = lerNumeroInteiro();
        if (procurarVerticePorID(grafo, id_vertice) == -1){
            break;
        }
        else {
            printf("\n\tVERICE COM ID %d JÁ EXISTE! DIGITE OUTRO!\n", id_vertice);
        }
    }

    adicionarVertice(grafo, criaVertice(id_vertice));

    printf("\nVERTICE %d ADICIONADO COM SUCESSO!\n", id_vertice);
    
    return;
}

void mostrarGrafo(Grafo *grafo){
    printf("\nIMPRIMINDO O GRAFO\n");
    imprimeGrafo(grafo);
    printf("\n");
}
void inserirAresta(Grafo *grafo){
    printf("\nINSERIR ARESTA");
    alterarAresta(grafo);
}

void imprimirIDVertices(Grafo *grafo) {
    if (grafo){
        for (int c = 0; c < grafo->numero_vertices; c++){
            printf("\n\tID: %d ", grafo->vertices[c].id_vertice);
        }
    }
}

void alterarAresta(Grafo *grafo){
    if (grafo == NULL) return;
    printf("\nALTERAR ARESTA");
    
    // Mostra todos o ID dos vértices do grafo
    printf("\nVertices do grafo");
    imprimirIDVertices(grafo);

    int id_vertice_01 = 0;
    do {
        printf("\nDigite o ID do primeiro vertice: ");
        id_vertice_01 = lerNumeroInteiro();
    } while (procurarVerticePorID(grafo, id_vertice_01) == -1);

    int id_vertice_02 = 0;
    do {
        printf("\nDigite o ID do segundo vertice: ");
        id_vertice_02 = lerNumeroInteiro();
    } while (procurarVerticePorID(grafo, id_vertice_02) == -1);

    Vertice *vertice_temp = &(grafo->vertices[procurarVerticePorID(grafo, id_vertice_01)]);
    if (verticeEhAdjacentePorID(*vertice_temp, id_vertice_02)){
        printf("\nOS VÉRTICES JÁ SÃO ADJACENTES \n");
        imprimeAresta(vertice_temp->arestas[procurarArestaPorID(*vertice_temp, id_vertice_02)]);
        
        float peso = 0.0f;
        printf("\nDigite o peso da nova conexão entre os vértices: ");
        peso = lerNumeroReal();

        // Como o grafo não é dirigido, então é adicionado a aresta aos dois vértices, a menos que os dois sejam o mesmo vértice
        modificaArestaPorID(*vertice_temp, id_vertice_02, peso);
        if (id_vertice_01 != id_vertice_02){
            vertice_temp = &(grafo->vertices[procurarVerticePorID(grafo, id_vertice_02)]);
            modificaArestaPorID(*vertice_temp, id_vertice_01, peso);
        }

        printf("\nAresta Modificada");
        return;
    }
    else {
        printf("\nAresta não existe, uma nova será criada");
        float peso = 0.0f;
        printf("\nDigite o peso da conexão entre os vértices: ");
        peso = lerNumeroReal();

        // Como o grafo não é dirigido, então é adicionado a aresta aos dois vértices, a menos que os dois sejam o mesmo vértice
        adicionarAresta(vertice_temp, criaAresta(id_vertice_01, id_vertice_02, peso));
        if (id_vertice_01 != id_vertice_02){
            vertice_temp = &(grafo->vertices[procurarVerticePorID(grafo, id_vertice_02)]);
            adicionarAresta(vertice_temp, criaAresta(id_vertice_02, id_vertice_01, peso));
        }

        printf("\nAresta adicionada");
    }

}
void gerarMST(Grafo *grafo){
    Grafo *grafo_mst = kruskal(grafo);
    imprimeGrafo(grafo_mst);
    liberaGrafo(grafo_mst);
}
void sair(Grafo *grafo){
    liberaGrafo(grafo);
    exit(0);
}

OPCAO_MENU menu(){
    OPCAO_MENU escolha;
    do {
        printf("\n");
        printf("\n\t%d INSERIR VERTICE", INSERIR_VERTICE);
        printf("\n\t%d MOSTRAR GRAFO", MOSTRAR_GRAFO);
        printf("\n\t%d INSERIR ARESTA", INSERIR_ARESTA);
        printf("\n\t%d ALTERAR ARESTA", ALTERAR_ARESTA);
        printf("\n\t%d MOSTRAR ARVORE GERADORA MINIMA", GERAR_MST);
        printf("\n\t%d SAIR", SAIR);
        printf("\n");
        escolha = lerNumeroInteiro();
    } while (escolha != INSERIR_VERTICE &&
             escolha != MOSTRAR_GRAFO &&
             escolha != INSERIR_ARESTA &&
             escolha != ALTERAR_ARESTA && 
             escolha !=  GERAR_MST &&
             escolha != SAIR
    );
    return escolha;
}

int lerNumeroInteiro(){
    int numero = 0;
    while (scanf("%d%*c", &numero) == 0) {
        while (getc(stdin) != '\n');
    }
    return numero;
}

float lerNumeroReal() {
    float numero = 0.0;
    while (scanf("%f%*c", &numero) == 0) {
        while (getc(stdin) != '\n');
    }
    return numero;
}

int comparador_de_arestas(void *aresta1, void *aresta2){
    if (((Aresta *) (aresta1))->peso < ((Aresta *) (aresta2))->peso) return -1;
    if (((Aresta *) (aresta1))->peso == ((Aresta *) (aresta2))->peso) return 0;
    return 1;
}

void funcao_troca_arestas(void *aresta1, void *aresta2){
    Aresta temp = * (Aresta *) aresta1;
    * (Aresta *) (aresta1) = * (Aresta *) (aresta2);
    * (Aresta *) (aresta2) = temp;
}

// Função que será utilizada para ordenar as arestas
// A função comparadora deve retornar -1 se o primeiro item for menor, 0 se for igual e 1 se for maior
void quicksort(void *dado, int tamanho, int (*funcao_comparadora)(void *, void *), void (*funcao_swap)(void *, void *), int tamanho_bytes){
    if (dado == NULL || tamanho <= 1) return;
    
    void *pivo = malloc(tamanho_bytes);
    if (pivo == NULL) return;

    pivo = memcpy(pivo, dado + ((rand() % tamanho) * tamanho_bytes), tamanho_bytes);

    int i = 0;
    int j = tamanho - 1;

    while (i <= j) {
        while (funcao_comparadora(dado + (i * tamanho_bytes), pivo) == -1) i++;
        while (funcao_comparadora(dado + (j * tamanho_bytes), pivo) == 1) j--;

        if (i <= j){
            funcao_swap(dado + (i * tamanho_bytes), dado + (j * tamanho_bytes));
            i++;
            j--;
        }
    }

    free(pivo);
    // Recursão
    quicksort(dado, j + 1, funcao_comparadora, funcao_swap, tamanho_bytes);
    quicksort((void *)(dado + i * tamanho_bytes), tamanho - i, funcao_comparadora, funcao_swap, tamanho_bytes);

}

Grafo *kruskal(Grafo *grafo){
    if (grafo == NULL) return NULL;


    // Retorna as arestas do grafo
    int numero_arestas = 0;
    int indice_proxima_aresta = 0;
    Aresta *arestas_grafo_original = NULL;

    for (int c = 0; c < grafo->numero_vertices; c++){
        // Aumenta o vetor das arestas
        numero_arestas += grafo->vertices[c].numero_arestas;
        arestas_grafo_original = (Aresta *) realloc(arestas_grafo_original, sizeof(Aresta) * numero_arestas);
        if (arestas_grafo_original == NULL) return NULL;

        for (int i = 0; i < grafo->vertices[c].numero_arestas; i++){
            arestas_grafo_original[indice_proxima_aresta++] = grafo->vertices[c].arestas[i];
        }
    }

    // Ordena as arestas 
    quicksort((void *)(arestas_grafo_original), numero_arestas, &comparador_de_arestas, &funcao_troca_arestas, sizeof(Aresta));


    // Cria uma floresta com os vértices inicialmente todos separados, e sem as arestas
    Grafo **floresta = (Grafo **) malloc(sizeof(Grafo *) * grafo->numero_vertices);
    if (floresta == NULL) return NULL;
    for (int c = 0; c < grafo->numero_vertices; c++){
        Grafo *temp = criaGrafo();
        
        adicionarVertice(temp, criaVertice(grafo->vertices[c].id_vertice));

        floresta[c] = temp;
    }

    for (int c = 0; c < numero_arestas; c++) {
        // Se estão em conjuntos separados, faz a união, e adiciona a aresta que estava ligando eles ao novo grafo
        Vertice vertice01;
        int indice_grafo_vertice_01;
        for (int contador = 0; contador < grafo->numero_vertices; contador++) {
            int pos_vertice = procurarVerticePorID(floresta[contador], arestas_grafo_original[c].id_vertice_01);
            if (pos_vertice != -1){
                vertice01 = floresta[contador]->vertices[pos_vertice];
                indice_grafo_vertice_01 = contador;
                break;
            }
        }

        Vertice vertice02;
        int indice_grafo_vertice_02;
        for (int contador = 0; contador < grafo->numero_vertices; contador++) {
            int pos_vertice = procurarVerticePorID(floresta[contador], arestas_grafo_original[c].id_vertice_02);
            if (pos_vertice != -1){
                vertice02 = floresta[contador]->vertices[pos_vertice];
                indice_grafo_vertice_02 = contador;
                break;
            }
        }

        if (indice_grafo_vertice_01 != indice_grafo_vertice_02){
            // FAZER A UNIÃO DOS GRAFOS DE CADA VÉRTICE, NÃO ESQUECER DE UNIR AS ARESTAS
            
            // Adiciona todos os vértices (e arestas) do grafo que contém o segundo vértice ao grafo que contém o primeiro vértice
            for (int i = 0; i < floresta[indice_grafo_vertice_02]->numero_vertices; i++){
                adicionarVertice(floresta[indice_grafo_vertice_01], floresta[indice_grafo_vertice_02]->vertices[i]);
            }
            
            // Deixa o grafo que continha o vértice 02 ZERADO
            liberaGrafo(floresta[indice_grafo_vertice_02]);
            floresta[indice_grafo_vertice_02] = criaGrafo();

            // Adiciona a aresta aos dois vértices
            adicionarAresta(&floresta[indice_grafo_vertice_01]->vertices[procurarVerticePorID(floresta[indice_grafo_vertice_01], vertice01.id_vertice)], criaAresta(arestas_grafo_original[c].id_vertice_01, arestas_grafo_original[c].id_vertice_02, arestas_grafo_original[c].peso));
            adicionarAresta(&floresta[indice_grafo_vertice_01]->vertices[procurarVerticePorID(floresta[indice_grafo_vertice_01], vertice02.id_vertice)], criaAresta(arestas_grafo_original[c].id_vertice_02, arestas_grafo_original[c].id_vertice_01, arestas_grafo_original[c].peso));

            // Se algum grafo da floresta contém todos os vértices, então o algoritmo já finalizou
            if (floresta[indice_grafo_vertice_01]->numero_vertices == grafo->numero_vertices){
                break;
            }
        }
    }

    free(arestas_grafo_original);

    // Pega o grafo que contém todos os vértices do grafo original, e vai liberando os outros
    Grafo *resultado = NULL;
    for (int c = 0; c < grafo->numero_vertices; c++){
        if (floresta[c]->numero_vertices == grafo->numero_vertices) {
            resultado = floresta[c];
        }
        else {
            liberaGrafo(floresta[c]);
        }
    }

    // Libera a floresta que foi criada
    free(floresta);

    return resultado;
}

bool conjuntosSeparados(Grafo **vetor_grafos, int tamanho, Vertice vertice01, Vertice vertice02){
    return indiceGrafoContemVertice(vetor_grafos, tamanho, vertice01) != indiceGrafoContemVertice(vetor_grafos, tamanho, vertice02);
}

int indiceGrafoContemVertice(Grafo **vetor_grafos, int tamanho, Vertice vertice){
    for (int c = 0; c < tamanho; c++){
        if (procurarVerticePorID(vetor_grafos[c], vertice.id_vertice) != -1){
            return c;
        }
    }
    return -1;
}