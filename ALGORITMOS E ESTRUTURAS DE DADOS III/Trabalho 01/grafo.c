#include "grafo.h"

// Funções de grafo
Grafo *criaGrafo(){
    Grafo *temp = (Grafo *) malloc (sizeof(Grafo));
    if (temp == NULL){
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

bool adicionarVertice(Grafo *grafo, Vertice vertice){
    if (grafo) {
        if (procurarVerticePorID(grafo, vertice.id_vertice) == -1){
            // Aumenta o vetor que guarda os Vértices
            grafo->numero_vertices++;
            grafo->vertices = (Vertice *) realloc(grafo->vertices, sizeof(Vertice) * grafo->numero_vertices);
            if (grafo->vertices == NULL) {
                grafo->numero_vertices = 0;
                return false;
            }

            grafo->vertices[grafo->numero_vertices - 1] = criaVertice(vertice.id_vertice);
            
            copiarArestasVertice(&grafo->vertices[grafo->numero_vertices - 1], &vertice);
            
            return true;
        }
    }
    return false;
}

bool removerVertice(Grafo *grafo, Vertice vertice){
    return removerVerticePorId(grafo, vertice.id_vertice);
}

bool removerVerticePorId(Grafo *grafo, int id_vertice){
    if (grafo) {
        int posicao_vertice = procurarVerticePorID(grafo, id_vertice); 
        if (posicao_vertice != -1){
            memmove(
                &grafo->vertices[posicao_vertice], 
                &grafo->vertices[posicao_vertice+1], 
                (grafo->numero_vertices - posicao_vertice) * sizeof(Vertice)
            );

            grafo->numero_vertices--;
            grafo->vertices = (Vertice *) realloc(grafo->vertices, grafo->numero_vertices * sizeof(Vertice));
            if (grafo->vertices == NULL && grafo->numero_vertices > 0){
                grafo->numero_vertices = 0;
            }

            return true;
        }
    }
    return false;
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

bool adicionarAresta(Vertice *vertice, Aresta aresta){
    if (vertice == NULL) {
        return false;
    }
    if (aresta.id_vertice_01 == vertice->id_vertice){
        vertice->numero_arestas++;
        vertice->arestas = (Aresta *) realloc(vertice->arestas, sizeof(Aresta) * vertice->numero_arestas);
        if (vertice->arestas == NULL){
            vertice->numero_arestas = 0;
            return false;
        }
        vertice->arestas[vertice->numero_arestas - 1] = aresta;
        return true;
    }
    return false;
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

bool removerAresta(Vertice *vertice, Aresta aresta){
    return removerArestaPorId(vertice, aresta.id_vertice_02);
}

bool removerArestaPorId(Vertice *vertice, int id_vertice_02){
    if (vertice){
        int posicao_aresta = procurarArestaPorID(*vertice, id_vertice_02); 
        if (posicao_aresta != -1){
            for (int c = posicao_aresta; c < vertice->numero_arestas - 1; c++){
                vertice->arestas[c] = vertice->arestas[c + 1]; 
            }

            vertice->numero_arestas--;
            vertice->arestas = (Aresta *) realloc(vertice->arestas, vertice->numero_arestas * sizeof(Aresta));
            if (vertice->arestas == NULL && vertice->numero_arestas > 0){
                vertice->numero_arestas = 0;
            }
            return true;
        }
    }
    return false;
}

void imprimeAresta(Aresta aresta_imprimir){
    printf("(%d,%d): %.2f", aresta_imprimir.id_vertice_01, aresta_imprimir.id_vertice_02, aresta_imprimir.peso);
}

// Funções do programa
void imprimirIDVertices(Grafo *grafo) {
    if (grafo){
        for (int c = 0; c < grafo->numero_vertices; c++){
            printf("\n\tID: %d ", grafo->vertices[c].id_vertice);
        }
    }
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
            // Adiciona todos os vértices (e arestas) do grafo que contém o segundo vértice ao grafo que contém o primeiro vértice
            for (int i = 0; i < floresta[indice_grafo_vertice_02]->numero_vertices; i++){
                adicionarVertice(floresta[indice_grafo_vertice_01], floresta[indice_grafo_vertice_02]->vertices[i]);
            }
            
            // Deixa o grafo que continha o vértice 02 ZERADO
            liberaGrafo(floresta[indice_grafo_vertice_02]);
            floresta[indice_grafo_vertice_02] = criaGrafo();

            // Adiciona a aresta aos dois vértices
            adicionarAresta(
                &(floresta[indice_grafo_vertice_01]->vertices[procurarVerticePorID(floresta[indice_grafo_vertice_01], arestas_grafo_original[c].id_vertice_01)]),
                criaAresta(arestas_grafo_original[c].id_vertice_01, arestas_grafo_original[c].id_vertice_02, arestas_grafo_original[c].peso)
            );
            adicionarAresta(
                &(floresta[indice_grafo_vertice_01]->vertices[procurarVerticePorID(floresta[indice_grafo_vertice_01], arestas_grafo_original[c].id_vertice_02)]), 
                criaAresta(arestas_grafo_original[c].id_vertice_02, arestas_grafo_original[c].id_vertice_01, arestas_grafo_original[c].peso)
            );

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
Grafo *lerDeArquivo(char *nome_arquivo, char *delimitador){
    FILE *arquivo = fopen(nome_arquivo, "rt");
    if (arquivo == NULL) {
        return NULL;
    }

    Grafo *grafo = criaGrafo();

    char *buffer_linha = NULL;
    size_t tamanho_buffer = 0;
    
    int contador_coluna = 0;
    int contador_linha = 0;
    char *token = NULL; 

    while(getline(&buffer_linha, &tamanho_buffer, arquivo) != EOF){
        adicionarVertice(grafo, criaVertice(contador_linha));   // Caso já exista o vértice ele só não adiciona
        contador_coluna = 0;

        token = strtok(buffer_linha, delimitador);
        while(token != NULL){
            adicionarVertice(grafo, criaVertice(contador_coluna));

            adicionarAresta(
                &(grafo->vertices[procurarVerticePorID(grafo, contador_linha)]), 
                criaAresta(contador_linha, contador_coluna, atof(token))
            );

            token = strtok(NULL, delimitador);

            contador_coluna++;
        }
        contador_linha++;
    }

    free(buffer_linha);
    fclose(arquivo);

    return grafo;
}

float pesoTotalGrafo(Grafo *grafo){
    float peso_total = 0.0;
    
    for (int c = 0; c < grafo->numero_vertices; c++) {
        for (int i = 0; i < grafo->vertices[c].numero_arestas; i++){
            peso_total += grafo->vertices[c].arestas[i].peso;
        }
    }

    return peso_total;
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

VetorInt visitarVertices(Grafo *grafo, Vertice vertice){
    VetorInt vetor = criaVetorInt();
    _visitarVertices(grafo, vertice, -1, &vetor);
    return vetor;
}

void _visitarVertices(Grafo *grafo, Vertice vertice, int id_vertice_pai, VetorInt *vetor_salvar){
    adicionaItemVetor(vetor_salvar, vertice.id_vertice);
    for (int c = 0; c < vertice.numero_arestas; c++){
        if (vertice.arestas[c].id_vertice_02 != id_vertice_pai){
            _visitarVertices(grafo, grafo->vertices[procurarVerticePorID(grafo, vertice.arestas[c].id_vertice_02)], vertice.id_vertice, vetor_salvar);
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
            criaAresta(id_v_01, id_v_02, peso)
        );
        adicionarAresta(
            &novo_grafo->vertices[(c + 1) % novo_grafo->numero_vertices],
            criaAresta(id_v_02, id_v_01, peso)
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