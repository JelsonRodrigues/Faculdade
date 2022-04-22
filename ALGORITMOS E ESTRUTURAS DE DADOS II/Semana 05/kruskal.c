#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>

/*
TODO

tenho que de alguma forma ir salvando as arestas quando adicionar um vértice a outro conjunto
e no final  recriar o grafo no formato de matriz 

Criar funções para modularizar o código e deixar ele mais limpo e legível
*/


/* Outros */
typedef unsigned int uint;
int lerNumeroInteiro();
float lerNumeroReal();

/* Menu */
typedef enum { ADICIONAR_NO = 1, MOSTRAR_GRAFO = 2, ALTERAR_ARESTAS = 3, GERAR_ARVORE_MINIMA = 4, SAIR = 5 } OpcoesMenu;
OpcoesMenu menu();
void limparEntrada(FILE *entrada);

/* Nodo */
typedef struct {
    char *nome_cidade;
} Nodo;
Nodo criaNodo(char *nome);
void imprimeNodo(Nodo *no);
Nodo lerNodo();

/* Grafo */
// A estrutura do grafo é composta de um vetor de nós e uma matriz com os pesos
typedef struct {
    Nodo *nos;
    uint numero_nos;
    float **pesos;
} Grafo;
Grafo *criaGrafo();
void adicionarNo(Grafo *grafo);
void inserirNoGrafo(Grafo *grafo, Nodo no);
bool nodoPertenceAoGrafo(Grafo *grafo, Nodo no);
void alterarConexoesNo(Grafo *grafo);
void alteraPesoAresta(Grafo *grafo, uint posicao_primeiro_no, uint posicao_segundo_no, float nova_conexao);
void imprimirGrafo(Grafo *g1);
void liberarGrafo(Grafo *g1);
uint maiorStringTamanho(Nodo *nos, int numero_nos);
int procuraNodo(Grafo *grafo, Nodo no);
typedef struct {
    int *verticesAdjacentes;
    int tamanho;
} VerticesAdjacentes;

VerticesAdjacentes verticesAdjacentes(Grafo *grafo, int posicao_no);

/* Kruskal */
typedef struct {
    int indice_vertice;
    int conjunto_pertence;
} AuxVertice;
typedef struct {
    int indice_no_01;
    int indice_no_02;
    float peso;
} Aresta;
typedef struct {
    Aresta *arestas;
    int tamanho;
} VetorArestas;
void mostrarArvoreMinima(Grafo *grafo);
Grafo *kruskal(Grafo *grafo);
void uniaoDeConjuntos(AuxVertice *vertices_auxiliar, int tamanho, int conjunto1, int conjunto2);
bool pertenceAoMesmoConjunto(AuxVertice *conjunto_vertices, int vertice01, int vertice02);
VetorArestas arestas_do_grafo(Grafo *grafo);
AuxVertice *incializaAuxVertices(Grafo *grafo);


/* Modelagem do grafo como uma matriz */
int main() {
    Grafo *g1 = criaGrafo();
    if (!g1) {
        printf("Não foi possível criar o grafo!\n");
        return EXIT_FAILURE;
    }

    OpcoesMenu escolha_menu = 0;
    while (true) {
        escolha_menu = menu();
        switch (escolha_menu)
        {
        case ADICIONAR_NO:
            adicionarNo(g1);
            break;
        case MOSTRAR_GRAFO:
            imprimirGrafo(g1);
            break;
        case ALTERAR_ARESTAS:
            alterarConexoesNo(g1);
            break;
        case GERAR_ARVORE_MINIMA:
            mostrarArvoreMinima(g1);
            break;
        case SAIR:
            liberarGrafo(g1);
            printf("SAINDO...\n");
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Opção errada!");
            break;
        }
    }

    return EXIT_SUCCESS;
}

int lerNumeroInteiro(){
    int numero = 0;
    while (scanf("%d%*c", &numero) == 0) {
        limparEntrada(stdin);
    }
    return numero;
}

float lerNumeroReal() { 
    float numero = 0.0f;
    while (scanf("%f%*c", &numero) == 0) {
        limparEntrada(stdin);
    }
    return numero;
}

Grafo *criaGrafo() {
    Grafo *temp = (Grafo *) malloc(sizeof(Grafo));
    if (temp) {
        temp->nos = NULL;
        temp->numero_nos = 0;
        temp->pesos = NULL;
    }
    return temp;
}

OpcoesMenu menu(){
    OpcoesMenu escolha;
    do {
        printf("\n");
        printf("\nDIGITE UMA OPÇÃO");
        printf("\n\t%d. ADICIONAR NÓ AO GRAFO", ADICIONAR_NO);
        printf("\n\t%d. MOSTRAR O GRAFO", MOSTRAR_GRAFO);
        printf("\n\t%d. ALTERAR ARESTAS", ALTERAR_ARESTAS);
        printf("\n\t%d. GERAR A ARVORE MÍNIMA", GERAR_ARVORE_MINIMA);
        printf("\n\t%d. SAIR", SAIR);
        printf("\n");
        escolha = lerNumeroInteiro();
    } while (escolha != ADICIONAR_NO &&
            escolha != MOSTRAR_GRAFO && 
            escolha != ALTERAR_ARESTAS &&
            escolha != GERAR_ARVORE_MINIMA && 
            escolha != SAIR);
    
    return escolha;
}

void limparEntrada(FILE *entrada){
    while (getc(entrada) != '\n');
}

Nodo criaNodo(char *nome){
    Nodo temp = {.nome_cidade=nome};
    return temp;
}
Nodo lerNodo() {
    Nodo no;
    
    char *nome = NULL;
    size_t numero_caracteres = 0;
    
    printf("\nDigite o nome da cidade: ");
    getline(&nome, &numero_caracteres, stdin);

    // Remove o enter do final
    int tamanho_nome = strlen(nome);
    if (nome[tamanho_nome - 1] == '\n') {
        nome[tamanho_nome - 1] = '\0';
    }

    no.nome_cidade = nome;
    return no;
}

void adicionarNo(Grafo *grafo){
    if (grafo == NULL) return;

    Nodo nodo_adicionar = lerNodo();
    inserirNoGrafo(grafo, nodo_adicionar);

    return;
}

void inserirNoGrafo(Grafo *grafo, Nodo no) {
    if (grafo){
        // Aumenta o vetor que contém os nós
        grafo->nos = realloc(grafo->nos, sizeof(Nodo) * (grafo->numero_nos +1));
        if (grafo->nos == NULL) {
            printf("Não foi possível adicionar o nó!");
            return;
        }
        grafo->nos[grafo->numero_nos] = no;
        grafo->numero_nos++;

        // Como o grafo vai ser não direcionado, então a matriz de representação é simétrica
        // Por isso eu vou salvar apenas a parte de baixo da matriz
        
        // Aumenta o número de linhas da matriz dos pesos
        grafo->pesos = realloc(grafo->pesos, sizeof(float *) * grafo->numero_nos);
        if (grafo->pesos == NULL) {
            printf("Não foi possível adicionar o nó!");
            return;
        }

        // Apenas a útima linha vai ter o mesmo número colunas que o número de nós no grafo
        grafo->pesos[grafo->numero_nos-1] = calloc(grafo->numero_nos, sizeof(float));
        if (grafo->pesos[grafo->numero_nos-1] == NULL) {
            printf("Não foi possível adicionar o nó!");
            return;
        }
    }
}

void imprimirGrafo(Grafo *g1) {
    if (g1){
        if (g1->numero_nos == 0) {
            printf("O grafo está vazio.\n");
            return;
        }

        uint tamanho_maior_nome = maiorStringTamanho(g1->nos, g1->numero_nos);
        if (tamanho_maior_nome < 4) tamanho_maior_nome += 4;

        // Mostra o nome de cada coluna
        printf("%*s", tamanho_maior_nome + 3,  " | ");
        for (uint c = 0; c < g1->numero_nos; c++){
            printf("%-*s | ", tamanho_maior_nome,  g1->nos[c].nome_cidade);
        }

        // Imprime cada linha e o conteúdo
        for (uint c = 0; c < g1->numero_nos; c++){
            printf("\n%*s | ", tamanho_maior_nome, g1->nos[c].nome_cidade);
            for (uint i = 0; i < g1->numero_nos; i++) {
                if (i <= c){
                    // Imprime o valor da linha
                    printf("%-*.3f | ", tamanho_maior_nome,  g1->pesos[c][i]);
                }
                else {
                    // Segue imprimindo o valor da coluna
                    printf("%-*.3f | ", tamanho_maior_nome,  g1->pesos[i][c]);
                }
            }
        }
    }
}

uint maiorStringTamanho(Nodo *nos, int numero_nos) {
    uint maior = 0;

    for (int c = 0; c < numero_nos; c++) {
        if (c == 0) {
            maior = strlen(nos[c].nome_cidade);
        }

        if (strlen(nos[c].nome_cidade) > maior) {
            maior = strlen(nos[c].nome_cidade);
        }
    }

    return maior;
}

void alterarConexoesNo(Grafo *grafo) {
    if (!grafo) return;
    for (uint c = 0; c < grafo->numero_nos; c++) {
        printf("\n%02u - %s", c+1,  grafo->nos[c].nome_cidade);
    }
    uint posicao_primeiro_no = 0;
    do {
        printf("\nDigite o número do primeiro nodo da conexão: ");
        posicao_primeiro_no = lerNumeroInteiro();
    } while (posicao_primeiro_no <= 0 ||  posicao_primeiro_no > grafo->numero_nos);
    
    uint posicao_segundo_no = 0;
    do {
        printf("\nDigite o número do segundo nodo da conexão: ");
        posicao_segundo_no = lerNumeroInteiro();
    } while (posicao_segundo_no <= 0 || posicao_segundo_no > grafo->numero_nos);

    printf("\n\nOs nodos selecionados foram");
    printf("\n%s", grafo->nos[posicao_primeiro_no-1].nome_cidade);
    printf("\n%s", grafo->nos[posicao_segundo_no-1].nome_cidade);
    
    // Deixa no primeiro nó o menor dos valores, e no segundo nó o maior
    if (posicao_primeiro_no > posicao_segundo_no){
        int aux = posicao_primeiro_no;
        posicao_primeiro_no = posicao_segundo_no;
        posicao_segundo_no = aux;
    }
    // O maior valor sempre estará na linha e o menor na coluna
    printf("\nO valor da conexão atual entre estes nodos é %.03f", grafo->pesos[posicao_segundo_no-1][posicao_primeiro_no-1]);
    printf("\n");

    float nova_conexao = 0.0;
    do {
        printf("Agora digite o novo valor da conexão: ");
        nova_conexao = lerNumeroReal();
    } while (nova_conexao < 0.0);

    alterarPesoAresta(grafo, posicao_primeiro_no, posicao_segundo_no, nova_conexao);

    return;
}

void alteraPesoAresta(Grafo *grafo, uint posicao_primeiro_no, uint posicao_segundo_no, float nova_conexao){
    // Deixa no primeiro nó o menor dos valores, e no segundo nó o maior
    if (posicao_primeiro_no > posicao_segundo_no){
        int aux = posicao_primeiro_no;
        posicao_primeiro_no = posicao_segundo_no;
        posicao_segundo_no = aux;
    }
    grafo->pesos[posicao_segundo_no-1][posicao_primeiro_no-1] = nova_conexao;
}

void liberarGrafo(Grafo *g1){
    if (g1){
        // Libera o espaço do nome da cidade
        for (uint c = 0; c < g1->numero_nos ; c++){
            free(g1->nos[c].nome_cidade);
        }
        free(g1->nos);
        g1->nos = NULL;

        // Libera a matriz com os pesos
        for (uint c = 0; c < g1->numero_nos ; c++){
            free(g1->pesos[c]);
        }
        free(g1->pesos);
        g1->pesos = NULL;
        g1->numero_nos = 0;
        
        free(g1);
    }
}

VerticesAdjacentes verticesAdjacentes(Grafo *grafo, int posicao_no){
    VerticesAdjacentes temp_v;
    temp_v.tamanho = 0;
    
    // Aloca espaço para o pior caso, ser adjacente a todos os vértices
    temp_v.verticesAdjacentes = (int *) malloc(sizeof(int) * grafo->numero_nos);
    if (!temp_v.verticesAdjacentes) {
        printf("ERRO\n");
        return temp_v;
    }

    // Salva o índice dos vértices que são adjacentes (peso da conexão diferente de zero)
    for (uint c = 0; c < grafo->numero_nos; c++){
        if (posicao_no <= c){
            if (grafo->pesos[posicao_no][c] != 0.0){
                temp_v.verticesAdjacentes[temp_v.tamanho++] = c;
            }
        }
        else {
            if (grafo->pesos[c][posicao_no] != 0.0){
                temp_v.verticesAdjacentes[temp_v.tamanho++] = c;
            }
        }
    }

    // Diminui o espaço que não está sendo utilizado
    temp_v.verticesAdjacentes = realloc(temp_v.verticesAdjacentes, sizeof(int) * temp_v.tamanho);
    if (!temp_v.verticesAdjacentes) {
        temp_v.tamanho = 0;
    }

    return temp_v;    
}

// Retorna o índice do nó, ou -1 caso não encontrar
int procuraNodo(Grafo *grafo, Nodo no){
    if (grafo) {
        for (uint c = 0; c < grafo->numero_nos; c++){
            if (strcmp(no.nome_cidade, grafo->nos[c].nome_cidade) == 0) return c;
        }
    }
    return -1;
}


void mostrarArvoreMinima(Grafo *grafo){
    if (grafo == NULL) return;
    printf("\nUma arvore geradora mínima é:\n");
    Grafo *res = kruskal(grafo);
    imprimirGrafo(res);
    liberarGrafo(res);

    printf("\n");
}

Grafo *kruskal( Grafo *grafo){
    if (grafo == NULL) return NULL;

    // Arestas do grafo de ordenada pelo peso
    VetorArestas arestas = arestas_do_grafo(grafo);

    // Coloca cada vértice em um conjunto separado
    AuxVertice *vertices_auxiliar = incializaAuxVertices(grafo);

    VetorArestas arestas_arvore_minima;
    arestas_arvore_minima.tamanho = (grafo->numero_nos -1);
    arestas_arvore_minima.arestas = (Aresta *) malloc(sizeof(Aresta) * arestas_arvore_minima.tamanho);
    if (arestas_arvore_minima.arestas == NULL){
        printf("Não foi possível alocar memória\n");
        return NULL;
    }
    int prox_posicao_aresta = 0;

    for (int c = 0; c < arestas.tamanho; c++){
        if (!pertenceAoMesmoConjunto(vertices_auxiliar, arestas.arestas[c].indice_no_01, arestas.arestas[c].indice_no_02)) {
            // Aqui eu tenho que adicionar a aresta que liga os dois na nova arvore, e depois eu tenho que recontruir o grafo com as arestas daqui
            uniaoDeConjuntos(vertices_auxiliar, grafo->numero_nos, vertices_auxiliar[arestas.arestas[c].indice_no_01].conjunto_pertence, vertices_auxiliar[arestas.arestas[c].indice_no_02].conjunto_pertence);
            arestas_arvore_minima.arestas[prox_posicao_aresta] = arestas.arestas[c];
            prox_posicao_aresta++;
        }
    }

    for (int c = 0; c < arestas_arvore_minima.tamanho; c++){
        printf("(%d, %d) -> %f\n", arestas_arvore_minima.arestas[c].indice_no_01, arestas_arvore_minima.arestas[c].indice_no_02, arestas_arvore_minima.arestas[c].peso);
    }


    // Recriar o grafo no formato de matriz
    return criaGrafo();

}

// Retorna um vetor com as arestas não nulas do grafo
VetorArestas arestas_do_grafo(Grafo *grafo) {
    VetorArestas temp;
    temp.tamanho = 0;
    temp.arestas = NULL;

    for (int c = 0; c < grafo->numero_nos; c++) {
        for (int i = 0; i <= c; i++) {
            if (grafo->pesos[c][i] != 0.0) {
                temp.tamanho++;
                // Aumenta o tamanho do vetor arestas
                temp.arestas = realloc(temp.arestas, sizeof(Aresta) * (temp.tamanho + 1));
                
                // Insere as arestas de modo já ordenado (como na fila de prioridade)
                int posicao = temp.tamanho-1;
                while (posicao > 0 && grafo->pesos[c][i] < temp.arestas[posicao].peso) {
                    // Desloca para a direita no vetor
                    temp.arestas[posicao + 1] = temp.arestas[posicao];
                    posicao--;
                }
                
                temp.arestas[posicao].peso = grafo->pesos[c][i];
                temp.arestas[posicao].indice_no_01 = c;
                temp.arestas[posicao].indice_no_02 = i;
            }
        }
    }

    return temp;
}


AuxVertice *incializaAuxVertices(Grafo *grafo){
    AuxVertice *temp = (AuxVertice *) malloc(sizeof(AuxVertice) * grafo->numero_nos);
    if (temp == NULL){
        printf("\nERRO AO ALOCAR MEMÓRIA\n");
        return NULL;
    }

    for (int c = 0; c < grafo->numero_nos; c++){
        temp[c].indice_vertice = c;
        temp[c].conjunto_pertence = c;
    }

    return temp;
}
bool pertenceAoMesmoConjunto(AuxVertice *conjunto_vertices, int vertice01, int vertice02){
    return conjunto_vertices[vertice01].conjunto_pertence == conjunto_vertices[vertice02].conjunto_pertence;
}

void uniaoDeConjuntos(AuxVertice *vertices_auxiliar, int tamanho, int conjunto1, int conjunto2) {
    for (int c = 0; c < tamanho; c++) {
        if (vertices_auxiliar[c].conjunto_pertence == conjunto1){
            vertices_auxiliar[c].conjunto_pertence = conjunto2;
        }
    }
}