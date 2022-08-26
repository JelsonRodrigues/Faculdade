#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

/* 
    A velocidade do programa será muito maior se eu implementar o vetor de vértices (grafo)
    e o vetor de arestar (vertice) de forma ordenada e realizar as buscas por procura binária
    Pra fazer isso é bem fácil na verdade, após a leitura dos valores do arquivo é só rodar um
    quicksort, ou alterar as funções de inserir aresta e vertice para inserir de forma ordenada.
    
    Ou se eu souber de antemão o número máximo de vértices e/ou de arestas eu possso implementar
    como uma hashtable que a velocidade seria maior ainda (a maior de todas na verdade)
    Na verdade como eu leio do arquivo o grafo e a cada linha/coluna é um vértice, dá pra mim
    criar uma hash pequena de início e como os valores de vértice são sempre incrementais (sempre diferentes),
    quando estiver enchendo a tabela eu aumento, não vai haver colisão neste caso. 
    Já para as arestas não é bem assim porque os valores não são incrementais (não necessariamente).
*/


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
Grafo *lerDeArquivo(char *nome_arquivo);
void liberaGrafo(Grafo *grafo_liberar);
void imprimeGrafo(Grafo *grafo_imprimir);
void adicionarVertice(Grafo *grafo, Vertice vertice);
void removerVertice(Grafo *grafo, Vertice vertice);
void removerVerticePorId(Grafo *grafo, int id_vertice);
int procurarVerticePorID(Grafo *grafo_procurar, int id_vertice);
float pesoTotalGrafo(Grafo *grafo);
int grauDoVertice(Vertice vertice);

// Funções de Vértice
Vertice criaVertice(int id_vertice);
void liberaVertice(Vertice *vertice_liberar);
void imprimeVertice(Vertice vertice_imprimir);
void adicionarAresta(Vertice *vertice, Aresta aresta);
void removerAresta(Vertice *vertice, Aresta aresta);
void removerArestaPorId(Vertice *vertice, int id_vertice_02);
bool verticeEhAdjacente(Vertice vertice_origem, Vertice vertice);
bool verticeEhAdjacentePorID(Vertice vertice_origem, int id_vertice);
int procurarAresta(Vertice vertice_origem, Vertice vertice_destino);
int procurarArestaPorID(Vertice vertice_origem, int id_vertice_destino);
void imprimirIDVertices(Grafo *grafo);
void copiarArestasVertice(Vertice *vertice_destino, Vertice *vertice_origem);

// Funções de Arestas (Conexão)
Aresta criaAresta(int id_vertice_01, int id_vertice_02, float peso);
void imprimeAresta(Aresta aresta_imprimir);

// Funções do programa
typedef enum {LER_VERTICES_DE_ARQUIVO=1, MOSTRAR_GRAFO, MENOR_CICLO_HAMILTONIANO, SAIR} OPCAO_MENU;
OPCAO_MENU menu();
int lerNumeroInteiro();
float lerNumeroReal();

// Opção do menu 
void mostrarGrafo(Grafo *grafo);
void menorCicloHamiltoniano(Grafo *grafo);
void sair(Grafo *grafo);


// Algoritmo de Kruskal
void quicksort(void *dado, int tamanho, int (*funcao_comparadora)(void *, void *), int tamanho_bytes);
Grafo *kruskal(Grafo *grafo);
bool conjuntosSeparados(Grafo **vetor_grafos, int tamanho, Vertice vertice01, Vertice vertice02);
int indiceGrafoContemVertice(Grafo **vetor_grafos, int tamanho, Vertice vertice);
int indiceGrafoContemVerticePorID(Grafo **vetor_grafos, int tamanho, int id_vertice);

// Algoritmo de Christofides
typedef struct {
    int *vetor;
    int tamanho;
} VetorInt;
Grafo *subgrafoVerticesGrauImparSemArestas(Grafo *grafo);
Grafo *subgrafoVerticesGrauImpar(Grafo *grafo);
VetorInt cicloEuleriano(Grafo *grafo, Vertice vertice_inicial);
int numeroArestasNoGrafo(Grafo *grafo);
int procurarArestaVetor(Aresta *vetor_arestas, int tamanho_vetor, Aresta aresta_procurar);

void adicionaItemVetor(VetorInt *vetor_adicionar, int valor_adicionar);
int procuraItemVetor(VetorInt *vetor_procurar, int valor_procurar);
void removerItemVetor(VetorInt *vetor_excluir, int item_excluir);
void inserirItemNaPosicao(VetorInt *vetor_inserir, int posicao, int valor_inserir);
int pegarItemDoVetor(VetorInt *vetor, int posicao);
VetorInt criaVetorInt();
bool estaVazio(VetorInt *vetor);
void liberarVetorInt(VetorInt *vetor);
int removerItemVetorPorPosicao(VetorInt *vetor, int posicao);
void visitarVertices(Grafo *grafo, Vertice vertice, int id_vertice_pai, VetorInt *vetor_salvar);
Grafo *recriaGrafo(Grafo *grafo_original, VetorInt *sequencia_vertices);

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
        case LER_VERTICES_DE_ARQUIVO:
            liberaGrafo(grafo);
            grafo = lerDeArquivo("./tsp1_253.txt");
            break;
        case MOSTRAR_GRAFO:
            mostrarGrafo(grafo);
            break;
        case MENOR_CICLO_HAMILTONIANO:
            menorCicloHamiltoniano(grafo);
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

            grafo->vertices[grafo->numero_vertices - 1] = criaVertice(vertice.id_vertice);
            
            copiarArestasVertice(&grafo->vertices[grafo->numero_vertices - 1], &vertice);
        }
        else {
            printf("\nVertice com o id %d ja existe no grafo.", vertice.id_vertice);
        }
    }
}

void removerVertice(Grafo *grafo, Vertice vertice){
    removerVerticePorId(grafo, vertice.id_vertice);
}

void removerVerticePorId(Grafo *grafo, int id_vertice){
    if (grafo) {
        int posicao_vertice = procurarVerticePorID(grafo, id_vertice); 
        if (posicao_vertice != -1){
            for (int c = posicao_vertice; c < grafo->numero_vertices - 1; c++){
                grafo->vertices[c] = grafo->vertices[c + 1]; 
            }

            grafo->numero_vertices--;
            grafo->vertices = (Vertice *) realloc(grafo->vertices, grafo->numero_vertices * sizeof(Vertice));
            if (grafo->vertices == NULL && grafo->numero_vertices > 0){
                printf("\nERRO AO EXCLUIR, TODOS VERTICES EXCLUIDOS!!! ;(");
                grafo->numero_vertices = 0;
            }
        }
        else {
            printf("\nO vertice nao esta presente no grafo!!!");
        }
    }
}

void copiarArestasVertice(Vertice *vertice_destino, Vertice *vertice_origem){
    if (vertice_destino && vertice_origem){
        for (int c = 0; c < vertice_origem->numero_arestas; c++){
            adicionarAresta(vertice_destino, vertice_origem->arestas[c]);
        }
    }
}

int procurarVerticePorID(Grafo *grafo_procurar, int id_vertice){
    if (grafo_procurar) {
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
        printf("\nVertice invalido"); 
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

void removerAresta(Vertice *vertice, Aresta aresta){
    removerArestaPorId(vertice, aresta.id_vertice_02);
}

void removerArestaPorId(Vertice *vertice, int id_vertice_02){
    if (vertice){
        int posicao_aresta = procurarArestaPorID(*vertice, id_vertice_02); 
        if (posicao_aresta != -1){
            for (int c = posicao_aresta; c < vertice->numero_arestas - 1; c++){
                vertice->arestas[c] = vertice->arestas[c + 1]; 
            }

            vertice->numero_arestas--;
            vertice->arestas = (Aresta *) realloc(vertice->arestas, vertice->numero_arestas * sizeof(Aresta));
            if (vertice->arestas == NULL && vertice->numero_arestas > 0){
                printf("\nERRO AO EXCLUIR, TODAS ARESTAS EXCLUIDAS!!! ;(");
                vertice->numero_arestas = 0;
            }
        }
        else {
            printf("\nA aresta nao esta presente no vertice!!!");
        }
    }
}

void imprimeAresta(Aresta aresta_imprimir){
    printf("(%d,%d): %.2f", aresta_imprimir.id_vertice_01, aresta_imprimir.id_vertice_02, aresta_imprimir.peso);
}

// Funções do programa
void mostrarGrafo(Grafo *grafo){
    printf("\nIMPRIMINDO O GRAFO\n");
    imprimeGrafo(grafo);
    printf("\n");
}

void imprimirIDVertices(Grafo *grafo) {
    if (grafo){
        for (int c = 0; c < grafo->numero_vertices; c++){
            printf("\n\tID: %d ", grafo->vertices[c].id_vertice);
        }
    }
}

void sair(Grafo *grafo){
    liberaGrafo(grafo);
    exit(0);
}

OPCAO_MENU menu(){
    OPCAO_MENU escolha;
    do {
        printf("\n");
        printf("\n\t%d LER VERTICES DE ARQUIVO", LER_VERTICES_DE_ARQUIVO);
        printf("\n\t%d MOSTRAR GRAFO", MOSTRAR_GRAFO);
        printf("\n\t%d MOSTRAR MENOR CICLO HAMILTONIANO (TSP)", MENOR_CICLO_HAMILTONIANO);
        printf("\n\t%d SAIR", SAIR);
        printf("\n");
        escolha = lerNumeroInteiro();
    } while (escolha != LER_VERTICES_DE_ARQUIVO &&
             escolha != MOSTRAR_GRAFO &&
             escolha != MENOR_CICLO_HAMILTONIANO &&
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

// Função que será utilizada para ordenar as arestas
// A função comparadora deve retornar -1 se o primeiro item for menor, 0 se for igual e 1 se for maior
void quicksort(void *dado, int tamanho, int (*funcao_comparadora)(void *, void *), int tamanho_bytes){
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
            // Copia byte a byte de um local para o outro
            for (int c = 0; c < tamanho_bytes; c++){
                char aux = * (char *) (dado + (i * tamanho_bytes) + c);
                * (char *) (dado + (i * tamanho_bytes) + c) = * (char *) (dado + (j * tamanho_bytes) + c);
                * (char *) (dado + (j * tamanho_bytes) + c) = aux;
            }
            i++;
            j--;
        }
    }

    free(pivo);
    // Recursão
    quicksort(dado, j + 1, funcao_comparadora, tamanho_bytes);
    quicksort((void *)(dado + i * tamanho_bytes), tamanho - i, funcao_comparadora, tamanho_bytes);
}

Grafo *kruskal(Grafo *grafo){
    if (grafo == NULL) return NULL;

    // Retorna as arestas do grafo
    int numero_arestas = 0;
    int indice_proxima_aresta = 0;
    Aresta *arestas_grafo_original = NULL;

    for(int c = 0; c < grafo->numero_vertices; c++){
        // Aumenta o vetor das arestas
        numero_arestas += grafo->vertices[c].numero_arestas;
        arestas_grafo_original = (Aresta *) realloc(arestas_grafo_original, sizeof(Aresta) * numero_arestas);
        if (arestas_grafo_original == NULL) return NULL;

        for (int i = 0; i < grafo->vertices[c].numero_arestas; i++){
            arestas_grafo_original[indice_proxima_aresta++] = grafo->vertices[c].arestas[i];
        }
    }

    // Ordena as arestas 
    quicksort((void *)(arestas_grafo_original), numero_arestas, &comparador_de_arestas, sizeof(Aresta));
    //qsort((void *)(arestas_grafo_original), (size_t) numero_arestas, sizeof(Aresta), &comparador_de_arestas);

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
        int indice_grafo_vertice_01 = indiceGrafoContemVerticePorID(floresta, grafo->numero_vertices, arestas_grafo_original[c].id_vertice_01);
        int indice_grafo_vertice_02 = indiceGrafoContemVerticePorID(floresta, grafo->numero_vertices, arestas_grafo_original[c].id_vertice_02);

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
            adicionarAresta(&(floresta[indice_grafo_vertice_01]->vertices[procurarVerticePorID(floresta[indice_grafo_vertice_01], arestas_grafo_original[c].id_vertice_01)]), criaAresta(arestas_grafo_original[c].id_vertice_01, arestas_grafo_original[c].id_vertice_02, arestas_grafo_original[c].peso));
            adicionarAresta(&(floresta[indice_grafo_vertice_01]->vertices[procurarVerticePorID(floresta[indice_grafo_vertice_01], arestas_grafo_original[c].id_vertice_02)]), criaAresta(arestas_grafo_original[c].id_vertice_02, arestas_grafo_original[c].id_vertice_01, arestas_grafo_original[c].peso));

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
    return indiceGrafoContemVerticePorID(vetor_grafos, tamanho, vertice.id_vertice);
}

int indiceGrafoContemVerticePorID(Grafo **vetor_grafos, int tamanho, int id_vertice){
    for (int c = 0; c < tamanho; c++){
        if (procurarVerticePorID(vetor_grafos[c], id_vertice) != -1){
            return c;
        }
    }
    return -1;
}


// Lê uma matriz de adjacência de um arquivo
Grafo *lerDeArquivo(char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "rt");
    if (arquivo == NULL) {
        printf("\nERRO AO LER ARQUIVO!!!");
        return NULL;
    }

    Grafo *grafo = criaGrafo();

    char *buffer_linha = NULL;
    size_t tamanho_buffer = 0;
    
    int contador_coluna = 0;
    int contador_linha = 0;
    char *delimitador = " ";
    char *token = NULL; 

    while(getline(&buffer_linha, &tamanho_buffer, arquivo) != EOF){
        adicionarVertice(grafo, criaVertice(contador_linha));   // Caso já exista o vértice ele só não adiciona
        contador_coluna = 0;

        token = strtok(buffer_linha, delimitador);
        while(token != NULL){
            adicionarVertice(grafo, criaVertice(contador_coluna));

            adicionarAresta(&(grafo->vertices[procurarVerticePorID(grafo, contador_linha)]), criaAresta(contador_linha, contador_coluna, atof(token)));

            token = strtok(NULL, delimitador);

            contador_coluna++;
        }
        contador_linha++;
    }

    free(buffer_linha);
    fclose(arquivo);

    return grafo;
}

void menorCicloHamiltoniano(Grafo *grafo){
    Grafo *mst = kruskal(grafo);
    imprimeGrafo(mst);

    printf("\n\nPeso do grafo original e de %.2f\n", pesoTotalGrafo(grafo));
    printf("\n\nPeso e de %.2f\n", pesoTotalGrafo(mst));

    /*
    Grafo *subgrafo_vertices_impares = subgrafoVerticesGrauImparSemArestas(mst);

    imprimeGrafo(subgrafo_vertices_impares);

    // Adiciona todas as arestas entre os vértices do subgrado com vértices de grau ímpar
    for (int c = 0; c < subgrafo_vertices_impares->numero_vertices; c++){
        int pos_vertice = procurarVerticePorID(grafo, subgrafo_vertices_impares->vertices[c].id_vertice);
        int num_arestas = 0;
        if (pos_vertice != -1) {
            num_arestas = grafo->vertices[pos_vertice].numero_arestas;
            for (int i = 0; i<num_arestas; i++) {
                if (subgrafo_vertices_impares->vertices[c].id_vertice != grafo->vertices[pos_vertice].arestas[i].id_vertice_02){
                    if (procurarVerticePorID(subgrafo_vertices_impares, grafo->vertices[pos_vertice].arestas[i].id_vertice_02) != -1){
                        adicionarAresta(&subgrafo_vertices_impares->vertices[c], grafo->vertices[pos_vertice].arestas[i]);
                    }
                }
            }
        }
    }

    imprimeGrafo(subgrafo_vertices_impares);
    liberaGrafo(subgrafo_vertices_impares);
    */
    //VetorInt ciclo_euleriano = cicloEuleriano(mst, mst->vertices[0]);
    VetorInt vertices_percorridos = criaVetorInt();
    int inicial = rand() % mst->numero_vertices;
    visitarVertices(mst, mst->vertices[inicial], -1, &vertices_percorridos);
    
    Grafo *ciclo_hamiltoniano = recriaGrafo(grafo, &vertices_percorridos);
    imprimeGrafo(ciclo_hamiltoniano);
    printf("\n\nPeso e de %.2f\n", pesoTotalGrafo(ciclo_hamiltoniano));
    liberaGrafo(ciclo_hamiltoniano);
    // Agora e so criar as arestas ligando cada um
    liberarVetorInt(&vertices_percorridos);
    liberaGrafo(mst);
}

float pesoTotalGrafo(Grafo *grafo){
    float peso_total = 0.0;
    
    for (int c = 0; c < grafo->numero_vertices; c++){
        for (int i = 0; i < grafo->vertices[c].numero_arestas; i++){
            peso_total += grafo->vertices[c].arestas[i].peso;
        }
    }

    return peso_total / (float) 2.0;    // Tem que dividir senão a contagem é feita duplicada
}

int grauDoVertice(Vertice vertice){
    return vertice.numero_arestas;
}

// Esta função retorna um grafo contendo apenas os vértices do grafo passado 
// que tenha um grau impar. Os vértices são retornados sem arestas
Grafo *subgrafoVerticesGrauImparSemArestas(Grafo *grafo){
    Grafo *resultado = criaGrafo();
    for (int c = 0; c < grafo->numero_vertices; c++){
        if (grauDoVertice(grafo->vertices[c]) % 2 != 0){
            adicionarVertice(resultado, criaVertice(grafo->vertices[c].id_vertice));
        }
    }
    return resultado;
}

// Esta função retorna um grafo contendo apenas os vértices do grafo passado 
// que tenha um grau impar. Os vértices são retornados com as arestas normalmente
Grafo *subgrafoVerticesGrauImpar(Grafo *grafo){
    Grafo *resultado = criaGrafo();
    for (int c = 0; c < grafo->numero_vertices; c++){
        if (grauDoVertice(grafo->vertices[c]) % 2 != 0){
            adicionarVertice(resultado, grafo->vertices[c]);
        }
    }
    return resultado;
}


VetorInt cicloEuleriano(Grafo *grafo, Vertice vertice_inicial){
    VetorInt ciclo = criaVetorInt();
    // Criar um grafo e ir seguindo o caminho das arestas??
    if (grafo == NULL) return ciclo;

    const int posciao_vertice_inicial = procurarVerticePorID(grafo, vertice_inicial.id_vertice);
    if (posciao_vertice_inicial == -1) {
        printf("\nO vertice inicial nao esta no grafo passado");
        return ciclo;
    }

    
    int numero_arestas_grafo = numeroArestasNoGrafo(grafo);
    int posicao_vertice_atual = posciao_vertice_inicial;
    int num_vertices_grafo = grafo->numero_vertices;
    
    // Salvo os vertices que ja foram visitados
    int num_arestas_percorridas = 0;
    Aresta *arestas_percorridas = (Aresta *) malloc(numero_arestas_grafo * sizeof(Aresta));
    if (arestas_percorridas == NULL){
        printf("\n ERRO AO ALOCAR MEMORIA");
        return ciclo;
    }
    memset(arestas_percorridas, 0, numero_arestas_grafo * sizeof(Aresta));

    VetorInt pilha = criaVetorInt();

    VetorInt vertices_visitados = criaVetorInt();
    
    while (num_arestas_percorridas != numero_arestas_grafo && vertices_visitados.tamanho != num_vertices_grafo) {
        adicionaItemVetor(&ciclo, grafo->vertices[posicao_vertice_atual].id_vertice);
        printf("%d ", grafo->vertices[posicao_vertice_atual].id_vertice);

        inserirItemNaPosicao(&pilha, -1, posicao_vertice_atual);

        bool alterado = false;
        for (int c = 0; c < grafo->vertices[posicao_vertice_atual].numero_arestas; c++){
            if (procuraItemVetor(&vertices_visitados, grafo->vertices[posicao_vertice_atual].arestas[c].id_vertice_02) == -1){
                alterado = true;
                adicionaItemVetor(&vertices_visitados, grafo->vertices[posicao_vertice_atual].id_vertice);
                arestas_percorridas[num_arestas_percorridas++] = grafo->vertices[posicao_vertice_atual].arestas[c];
                posicao_vertice_atual = procurarVerticePorID(grafo, grafo->vertices[posicao_vertice_atual].arestas[c].id_vertice_02);
                break;
            }
            if (procurarArestaVetor(arestas_percorridas, num_arestas_percorridas, grafo->vertices[posicao_vertice_atual].arestas[c]) == -1){
                // Aresta nao percorrida
                alterado = true;
                arestas_percorridas[num_arestas_percorridas++] = grafo->vertices[posicao_vertice_atual].arestas[c];
                posicao_vertice_atual = procurarVerticePorID(grafo, grafo->vertices[posicao_vertice_atual].arestas[c].id_vertice_02);
                break;
            }
        }
        if (alterado == false){
            posicao_vertice_atual = pegarItemDoVetor(&pilha, -1);
            removerItemVetorPorPosicao(&pilha, -1);
        }
    }
    free(arestas_percorridas);
    return ciclo;
}

void visitarVertices(Grafo *grafo, Vertice vertice, int id_vertice_pai, VetorInt *vetor_salvar){
    printf("%d ", vertice.id_vertice);
    adicionaItemVetor(vetor_salvar, vertice.id_vertice);
    for (int c = 0; c < vertice.numero_arestas; c++){
        if (vertice.arestas[c].id_vertice_02 != id_vertice_pai){
            visitarVertices(grafo, grafo->vertices[procurarVerticePorID(grafo, vertice.arestas[c].id_vertice_02)], vertice.id_vertice, vetor_salvar);
        }
    }
}

Grafo *recriaGrafo(Grafo *grafo_original, VetorInt *sequencia_vertices){
    if (grafo_original == NULL || sequencia_vertices == NULL) return NULL;
    Grafo *novo_grafo = criaGrafo();
    for (int c = 0; c < sequencia_vertices->tamanho; c++) {
        adicionarVertice(novo_grafo, criaVertice(sequencia_vertices->vetor[c]));
    }

    for (int c = 0; c < novo_grafo->numero_vertices; c++){
        int posicao_vertice_01 = procurarVerticePorID(grafo_original, novo_grafo->vertices[c % novo_grafo->numero_vertices].id_vertice);
        int posicao_vertice_02 = procurarVerticePorID(grafo_original, novo_grafo->vertices[(c + 1) % novo_grafo->numero_vertices].id_vertice);
        int posicao_aresta = procurarAresta(grafo_original->vertices[posicao_vertice_01], grafo_original->vertices[posicao_vertice_02]);
        int id_v_01 = grafo_original->vertices[posicao_vertice_01].id_vertice;
        int id_v_02 = grafo_original->vertices[posicao_vertice_02].id_vertice;
        float peso = grafo_original->vertices[posicao_vertice_01].arestas[posicao_aresta].peso;

        adicionarAresta(
            &novo_grafo->vertices[c % novo_grafo->numero_vertices],
            criaAresta(
                id_v_01,
                id_v_02,
                peso
            )
        );
        adicionarAresta(
            &novo_grafo->vertices[(c + 1) % novo_grafo->numero_vertices],
            criaAresta(
                id_v_02,
                id_v_01,
                peso
            )
        );
    }    

    return novo_grafo;
}

int numeroArestasNoGrafo(Grafo *grafo){
    int numero_arestas = 0;
    if (grafo) {
        for (int c = 0; c < grafo->numero_vertices; c++){
            numero_arestas += grafo->vertices[c].numero_arestas;
        }
    }
    return numero_arestas;
}

int procurarArestaVetor(Aresta *vetor_arestas, int tamanho_vetor, Aresta aresta_procurar){
    if (vetor_arestas == NULL) return -1;
    for (int c = 0; c < tamanho_vetor; c++) {
        if (
            vetor_arestas[c].id_vertice_01 == aresta_procurar.id_vertice_01 &&
            vetor_arestas[c].id_vertice_02 == aresta_procurar.id_vertice_02 &&
            vetor_arestas[c].peso == aresta_procurar.peso
        ){
            return c;
        }
    }

    return -1;
}

void adicionaItemVetor(VetorInt *vetor_adicionar, int valor_adicionar){
    if (vetor_adicionar){
        vetor_adicionar->tamanho++;
        vetor_adicionar->vetor = realloc(vetor_adicionar->vetor, vetor_adicionar->tamanho * sizeof(int));
        if (vetor_adicionar->vetor){
            vetor_adicionar->vetor[vetor_adicionar->tamanho - 1] = valor_adicionar;
        }
        else{
            vetor_adicionar->tamanho = 0;
        }
    }
}
int procuraItemVetor(VetorInt *vetor_procurar, int valor_procurar){
    if (vetor_procurar){
        for (int c = 0; c < vetor_procurar->tamanho; c++){
            if (valor_procurar == vetor_procurar->vetor[c]){
                return c;
            }
        }
    }
    return -1;
}
void removerItemVetor(VetorInt *vetor_excluir, int item_excluir){
    if (vetor_excluir){
        removerItemVetorPorPosicao(vetor_excluir, procuraItemVetor(vetor_excluir, item_excluir));
    }
}
int removerItemVetorPorPosicao(VetorInt *vetor, int posicao) {
    if (vetor){
        if (posicao >=0 && posicao < vetor->tamanho){
            int valor = vetor->vetor[posicao];
            memmove(&vetor->vetor[posicao], &vetor->vetor[posicao + 1], sizeof(int) * (vetor->tamanho - posicao));
            vetor->tamanho--;
            vetor->vetor = realloc(vetor->vetor, vetor->tamanho * sizeof(int));
            if (vetor->vetor == NULL){
                vetor->tamanho = 0;
            }
            return valor;
        }   
    }
    return 0;
}
int pegarItemDoVetor(VetorInt *vetor, int posicao){
    if (vetor){
        if (posicao < 0){
            posicao += vetor->tamanho;
        }
        if (posicao >=0 && posicao < vetor->tamanho){
            return vetor->vetor[posicao];
        }
    }
    return 0;
}
void inserirItemNaPosicao(VetorInt *vetor_inserir, int posicao, int valor_inserir){
    if (vetor_inserir){
        if (posicao < 0) {
            posicao += vetor_inserir->tamanho;
        }
        if (posicao < vetor_inserir->tamanho && posicao >= 0){
            if (posicao == vetor_inserir->tamanho - 1){
                adicionaItemVetor(vetor_inserir, valor_inserir);
                return;
            }
            vetor_inserir->tamanho++;
            vetor_inserir->vetor = realloc(vetor_inserir->vetor, vetor_inserir->tamanho * sizeof(int));
            if (vetor_inserir->vetor == NULL){
                vetor_inserir->tamanho = 0;
                return;
            }
            memmove(&vetor_inserir[posicao + 1], &vetor_inserir[posicao], sizeof(int) * (vetor_inserir->tamanho - posicao));
            vetor_inserir->vetor[posicao] = valor_inserir;
        }
    }
}
VetorInt criaVetorInt(){
    VetorInt temp = {  
        .tamanho = 0,
        .vetor = NULL
    };
    return temp;
}
bool estaVazio(VetorInt *vetor){
    if (vetor->tamanho == 0) return true;
    return false;
}
void liberarVetorInt(VetorInt *vetor){
    if (vetor){
        if (vetor->vetor){
            free(vetor->vetor);
            vetor->vetor = NULL;
        }
        vetor->tamanho = 0;
    }
}